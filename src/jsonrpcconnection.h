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
        return m_tag < other. m_tag;
    }

    virtual void parseReplyJson(const QJsonDocument& json );

    int tag() const
    {
        return m_tag;
    }

public slots:
    virtual QByteArray make();

protected slots:
    void setTag(int arg)
    {
        if (m_tag != arg) {
            m_tag = arg;
            emit tagChanged(arg);
            request.object["tag"] = m_tag;
        }
    }
signals:
    void tagChanged(int arg);
};

class JsonRpcConnection : public RpcConnection
{
    Q_OBJECT
public:
    explicit JsonRpcConnection(QUrl server, QObject *parent = 0);

    QMap<int, RpcCommand*> openCommands;
signals:

public slots:


    void sendCommand(RpcCommand* c){
        JsonRpcCommand* command = (JsonRpcCommand*) c;

        static int currentTag=7;
        //start at 7, just for fun
        //it is static because that helps detect migled messages with two RpcConnections


        command->setTag(currentTag++);
        openCommands.insert(command->tag(), command);

        //only allow one pending command until we have a cookie
        if(sessidCookie.isEmpty() && (openCommands.size()>1))
        {
            qDebug() << "skipping " << command->tag() << " because we have no cookie yet";
            return;
        }


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




virtual void gotReply(QNetworkReply *reply);

void flushBackloggedCommands();


};

#endif // JSONRPCCONNECTION_H
