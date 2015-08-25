#include "rpcconnection.h"
#include <QDomDocument>

#ifndef XMLRPCCONNECTION_H
#define XMLRPCCONNECTION_H

class XmlRpcCommand : public RpcCommand {
Q_OBJECT

protected:
    QVariantList requestArguments;

public:
    XmlRpcCommand(char *method, QObject *parent=0):
    RpcCommand(method, parent)
    {


    }



    // RpcCommand interface
public slots:
    virtual QByteArray make();
    virtual void gotReply();
    virtual void handleReply();

};

class XmlRpcConnection : public RpcConnection
{
    Q_OBJECT
public:
    explicit XmlRpcConnection(QString url, QObject *parent = 0);


signals:
    void error(QString message);

public slots:

};

#endif // XMLRPCCONNECTION_H
