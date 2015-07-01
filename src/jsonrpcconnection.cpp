#include "jsonrpcconnection.h"


JsonRpcConnection::JsonRpcConnection(QUrl server, QObject *parent) :
    RpcConnection(server, parent)
{
    cookieIsSet = false;
    contentType = "application/json";
}

void JsonRpcConnection::flushBackloggedCommands(){
    qDebug() << "flushing backlog of " << openCommands.size();
    if (openCommands.empty()) return;

    int lastTag = openCommands.lastKey();
    int currentTag=openCommands.firstKey();
    while(currentTag<=lastTag)
    {
        qDebug() << "sending command " << currentTag;
        sendCommand(openCommands.take(currentTag));
        currentTag++;
    }
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
        flushBackloggedCommands();
        return;
    }
    if(reply->error()){
        qWarning() << "got error " << reply->error() << reply->errorString();
        return;
    }

    QByteArray data = reply->readAll();
    if(data.length()==0){
        //qWarning() << "received empty";
        return;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if(error.error != QJsonParseError::NoError)
    {
        qWarning() << "parse error" << error.errorString();
        qWarning() << "at " << error.offset;
        qWarning() << doc;
        return;
    }
    int tag=doc.object()["tag"].toInt();
    qDebug() << "got Tag" << tag;

    JsonRpcCommand* command = (JsonRpcCommand*) openCommands.take(tag);
    if(!command){
        qWarning() << "received reply to unknown command";
        qWarning() << doc;
        return;
    }
    command->parseReplyJson(doc);

}


JsonRpcCommand::JsonRpcCommand(const char * method, QObject *parent)
    :
      RpcCommand(method, parent)
{

}

QByteArray JsonRpcCommand::make()
{
    request.object.remove("tag");
    request.object.insert("tag", tag());
    request.object["arguments"] = request.arguments;
    request.blob = QJsonDocument::fromVariant(request.object).toJson();
    qDebug() << "sending" << request.object["method"];
    return request.blob;
}

void JsonRpcCommand::parseReplyJson(const QJsonDocument &json ){
    reply.result   =json.object()["result"].toString();
    //TODO do something with result other than "success"
    reply.arguments=json.object()["arguments"].toObject().toVariantMap();
    handleReply();
    emit gotReply();
    deleteLater();
}
