#include "rpcconnection.h"
#include <QDomDocument>

#ifndef XMLRPCCONNECTION_H
#define XMLRPCCONNECTION_H

class XmlRpcCommand : public RpcCommand {
Q_OBJECT



    virtual void gotReply(const QDomDocument&  data);

    // RpcCommand interface
public slots:
    virtual QByteArray make();
    virtual void handleReply();

};

class XmlRpcConnection : public RpcConnection
{
    Q_OBJECT
public:
    explicit XmlRpcConnection(QUrl server, QObject *parent = 0);


signals:
    void error(QString message);

public slots:

};

#endif // XMLRPCCONNECTION_H
