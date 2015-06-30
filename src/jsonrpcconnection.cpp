#include "jsonrpcconnection.h"


JsonRpcConnection::JsonRpcConnection(QUrl server, QObject *parent) :
    RpcConnection(server, parent)
{
}

void JsonRpcConnection::flushBackloggedCommands(){
    qDebug() << "flushing backlog of " << openCommands.size();
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
    int statusCode =reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode==409)
    {
        qDebug() << "setting session cookie";
        sessidCookie=reply->rawHeader("X-Transmission-Session-Id");
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
    request.object["arguments"]=request.arguments;
    request.blob = QJsonDocument::fromVariant(request.object).toJson();

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
