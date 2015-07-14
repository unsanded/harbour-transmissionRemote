#include "rpcconnection.h"
#include <QString>
#include <stdlib.h>

#ifndef JSONRPCCONNECTION_H
#define JSONRPCCONNECTION_H

class JsonRpcConnection;

class JsonRpcCommand : public RpcCommand {

    friend class JsonRpcConnection;
    Q_OBJECT
    Q_PROPERTY(int tag READ tag WRITE setTag NOTIFY tagChanged)

    int m_tag;

public:

    JsonRpcCommand(const char* method, QObject* parent=0);

    virtual bool operator <(JsonRpcCommand& other){
        return m_tag < other.m_tag;
    }

    virtual void gotReply();

    int tag() const
    {
        return m_tag;
    }

public slots:
    virtual QByteArray make();

protected slots:
    void setTag(int arg);

signals:
    void tagChanged(int arg);
};

class JsonRpcConnection : public RpcConnection
{
     Q_OBJECT
protected:
    bool cookieIsSet ;
    QList <JsonRpcCommand*> backLog;
public:
    explicit JsonRpcConnection(QUrl server, QObject *parent = 0);

signals:

public slots:




    virtual void sendCommand(RpcCommand* c){
        static int currentTag = 0x42;

        JsonRpcCommand* command = (JsonRpcCommand*) c;
        command->setTag(currentTag++);

        //only send stuff if we have a cookie
        if(!cookieIsSet){

            backLog << command;
            //send the first command, otherwise we won't get a cookie
            if(backLog.size() > 1){
                    return;
            }
        }

        RpcConnection::sendCommand(command);
    }


void gotReply(QNetworkReply *reply);
};

#endif // JSONRPCCONNECTION_H
