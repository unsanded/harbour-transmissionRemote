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
    Q_OBJECT
protected:

   struct{
        QJsonObject object;
        QJsonObject arguments;
   } reply;

   struct{
        QJsonObject object;
        QJsonObject arguments;
   } request;


   friend class RpcConnection;
   virtual void parseReply();
   QNetworkReply* networkReply;

   int tag;

   public:
    RpcCommand(QString method){
        request.object["method"]=method;
    }

    bool operator<(RpcCommand& other){
        return tag< other.tag;
    }

public slots:

    virtual QByteArray make(){
        request.object["tag"]      =tag;
        request.object["arguments"]=request.arguments;
        return QJsonDocument(request.object).toJson();

    }

 signals:
    void gotReply();



};

class RpcConnection : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl server READ server WRITE setserver NOTIFY serverChanged)

    QUrl m_server;
    QByteArray sessidCookie;


    QNetworkAccessManager networkManager;

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


private:
void sendCommand(RpcCommand& command){
    QNetworkRequest request;
    request.setUrl(server());
    request.setRawHeader("X-Transmission-Session-Id", sessidCookie);
    QNetworkReply* reply = networkManager.sendCustomRequest(request, command.make());
    connect(
                &networkManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(gotReply(QNetworkReply*))
            );

}

};

#endif // RPCCONNECTION_H
