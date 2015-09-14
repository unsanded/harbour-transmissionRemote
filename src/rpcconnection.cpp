#include "rpcconnection.h"


RpcConnection::RpcConnection(QUrl server, QObject *parent) :
    QObject(parent),
    m_server(server),
    networkManager(this)
{
        contentType = "text/plain";
}

void RpcConnection::addBasicAuthorisation(QString username, QString password)
{
    QByteArray key = QString("%1:%2").arg(username, password).toStdString().c_str();
    QByteArray base64 = key.toBase64();
    QByteArray prefix("Basic ");
    addHeader("Authorization", prefix + base64);
}

void RpcConnection::sendCommand(RpcCommand *command){
    QNetworkRequest request ;
    request.setUrl(server());

    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);


    for(auto header : headers)
        request.setRawHeader(header.first, header.second);

    QNetworkReply* reply = networkManager.post(request,  command->make());

    command->networkReply=reply;
    connect(reply, &QNetworkReply::finished,
            command, &RpcCommand::gotReply
    );
}

void RpcConnection::setserver(QUrl arg)
{
    if (m_server != arg) {
        m_server = arg;
        emit serverChanged(arg);
    }
}




