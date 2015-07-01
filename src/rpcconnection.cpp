#include "rpcconnection.h"


RpcConnection::RpcConnection(QUrl server, QObject *parent) :
    QObject(parent),
    m_server(server),
    networkManager(this)
{
        connect(
                    &networkManager, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(gotReply(QNetworkReply*))
                    );
        contentType = "text/plain";
}

void RpcConnection::addBasicAuthorisation(QString username, QString password)
{
    QByteArray key = QString("%1:%2").arg(username, password).toStdString().c_str();
    QByteArray base64 = key.toBase64();
    QByteArray prefix("Basic ");
    addHeader("Authorization", prefix + base64);
//cGlldHBpcmFhdDpoZXJtYW5oZW5r
}




