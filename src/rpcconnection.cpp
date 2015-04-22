#include "rpcconnection.h"


RpcConnection::RpcConnection(QUrl server, QObject *parent) :
    QObject(parent),
    m_server(server),
    networkManager(this)
{


}

void RpcConnection::gotReply(QNetworkReply *reply)
{
    int statusCode =reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode==409)
    {
        qDebug() << "setting session cookie";
        sessidCookie=reply->rawHeader("X-Transmission-Session-Id");
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

    RpcCommand* command = openCommands.take(tag);
    if(!command){
        qWarning() << "received reply to unknown command";
        qWarning() << doc;
        return;
    }
    command->parseReplyJson(doc);

}

void RpcConnection::sendCommand(RpcCommand *command){
    static int currentTag=7;
    //start at 7, just for fun
    //it is static because that helps detect migled messages with two RpcConnections

    command->setTag(currentTag++);

    openCommands.insert(command->tag(), command);



    QNetworkRequest request ;
    request.setUrl(server());

    request.setRawHeader("X-Transmission-Session-Id", sessidCookie);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    networkManager.post(request,  command->make());
    connect(
        &networkManager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(gotReply(QNetworkReply*))
    );
}


void RpcCommand::parseReplyJson(const QJsonDocument &json ){
    reply.result   =json.object()["result"].toString();
    //TODO do something with result other than "success"
    reply.arguments=json.object()["arguments"].toObject();
    handleReply();
    emit gotReply();
    deleteLater();
}
