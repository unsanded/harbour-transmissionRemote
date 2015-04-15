#ifndef RPCCONNECTION_H
#define RPCCONNECTION_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>

class RpcConnection;

class RpcCommand: public QObject{
    Q_PROPERTY(int tag READ tag WRITE setTag NOTIFY tagChanged)
    Q_OBJECT
    int m_tag;

protected:

    bool preparationDone;
   struct{
        QJsonObject arguments;
        QString result ; // should contain "success" after response is parsed
   } reply;

   struct{
        QJsonObject object;
        QJsonObject arguments;
   } request;


   friend class RpcConnection;
   QNetworkReply* networkReply;


   public:
    RpcCommand(QString method, QObject* parent=0):
        QObject(parent)
    {
        request.object["method"]=method;
        preparationDone=false;
    }

    //to make commands sortable
    bool operator<(RpcCommand& other){
        return tag()< other.tag();
    }

    int tag() const
    {
        return m_tag;
    }

public slots:

    virtual QByteArray make()
    {
        if(!preparationDone){
            preparationDone=true;
            request.object["tag"]      =tag();
            request.object["arguments"]=request.arguments;
        }
        return QJsonDocument(request.object).toJson();
    }

    virtual void parseReplyJson(QJsonDocument& json){
        reply.result   =json.object()["result"].toString();
        //TODO do something with result other than "success"
        reply.arguments=json.object()["arguments"].toObject();
        handleReply();
        emit gotReply();
    }

    virtual void handleReply(){}

    void setTag(int arg)
    {
        if (m_tag != arg) {
            m_tag = arg;
            emit tagChanged(arg);
        }
    }

signals:
    void gotReply();



    void tagChanged(int arg);
};

class RpcConnection : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl server READ server WRITE setserver NOTIFY serverChanged)

    QUrl m_server;
    QByteArray sessidCookie;
    QNetworkAccessManager networkManager;

    QMap<int, RpcCommand*> openCommands;

public:
    explicit RpcConnection(QUrl server, QObject *parent = 0);



QUrl server() const
{
    return m_server;
}

signals:

void serverChanged(QUrl arg);

public slots:

void gotReply(QNetworkReply* reply);

void setserver(QUrl arg)
{
    if (m_server != arg) {
        m_server = arg;
        emit serverChanged(arg);
    }
}


public:
void sendCommand(RpcCommand* command);

};

#endif // RPCCONNECTION_H
