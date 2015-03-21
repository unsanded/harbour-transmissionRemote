#include "rpcconnection.h"

RpcConnection::RpcConnection(QUrl server, QObject *parent) :
    QObject(parent),
    networkManager(this),
    m_server(server)
{


}

void RpcConnection::gotReply(QNetworkReply *reply)
{
    if(reply->error()==409){
        sessidCookie=reply->rawHeader("X-Transmission-Session-Id");
    }

}


void RpcCommand::parseReply()
{

}
