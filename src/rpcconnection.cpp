#include "rpcconnection.h"


RpcConnection::RpcConnection(QUrl server, QObject *parent) :
    QObject(parent),
    m_server(server),
    networkManager(this)
{


}




