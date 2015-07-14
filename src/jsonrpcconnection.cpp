#include "jsonrpcconnection.h"


JsonRpcConnection::JsonRpcConnection(QUrl server, QObject *parent) :
    RpcConnection(server, parent)
{
        connect(
                    &networkManager, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(gotReply(QNetworkReply*))
                    );

    cookieIsSet = false;
    contentType = "application/json";
}


void JsonRpcConnection::gotReply(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode==409)
    {
        qDebug() << "setting session cookie";
        QString sessidCookie = reply->rawHeader("X-Transmission-Session-Id");

        addHeader("X-Transmission-Session-Id", sessidCookie.toStdString().c_str());
        cookieIsSet=true;
        //send the first backogged command to trigger a 200 status (see below)
        if(!backLog.empty())
            sendCommand(backLog.takeFirst());
        return;
    }
    if(reply->error()){
        qWarning() << "got error " << reply->error() << reply->errorString();
        return;
    }
    else
    {
        qDebug() << "got http status 200 . So Disconnecting onReply";
        disconnect(&networkManager,SIGNAL(finished(QNetworkReply*)), this, SLOT(gotReply(QNetworkReply*)));

        //now send all those commands that we missed out because of that stupid cookie
        for(JsonRpcCommand* command: backLog){
            sendCommand(command);
        }
        backLog.clear();
    }

}


JsonRpcCommand::JsonRpcCommand(const char * method, QObject *parent)
    :
      RpcCommand(method, parent)
{

}

QByteArray JsonRpcCommand::make()
{
    request.object.remove("tag");
    request.object["method"] = request.method;
    request.object.insert("tag", tag());
    request.object["arguments"] = request.arguments;
    request.blob = QJsonDocument::fromVariant(request.object).toJson();
    qDebug() << "sending" << request.object["method"];
    return request.blob;
}

void JsonRpcCommand::setTag(int arg)
{
    if (m_tag != arg) {
        m_tag = arg;
        emit tagChanged(arg);
        request.object["tag"] = m_tag;
    }
}

void JsonRpcCommand::gotReply(){
    qDebug() << "REPLY";
    if(networkReply->error())
        return;
    QJsonDocument json = QJsonDocument::fromJson(networkReply->readAll());

    reply.result = json.object()["result"].toString();
    //TODO do something with result other than "success"
    reply.arguments = json.object()["arguments"].toObject().toVariantMap();
    handleReply();
    deleteLater();
}
