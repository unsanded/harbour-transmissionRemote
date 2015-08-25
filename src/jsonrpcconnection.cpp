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

    requestObject.remove("tag");
    requestObject["method"] = method;
    requestObject.insert("tag", tag());
    requestObject["arguments"] = requestArguments;
    requestBlob = QJsonDocument(requestObject).toJson();
    qDebug() << "sending" << requestObject["method"];
    return requestBlob;

}

void JsonRpcCommand::setTag(int arg)
{
    if (m_tag != arg) {
        m_tag = arg;
        emit tagChanged(arg);
        requestObject["tag"] = m_tag;
    }
}

void JsonRpcCommand::gotReply(){
    qDebug() << "REPLY";
    if(networkReply->error())
        return;
    QJsonDocument json = QJsonDocument::fromJson(networkReply->readAll());

    result = json.object()["result"].toString();
    //TODO do something with result other than "success"
    replyArguments = json.object()["arguments"].toObject().toVariantMap();
    handleReply();
    deleteLater();
}
