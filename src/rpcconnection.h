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

    friend class RpcConnection;
protected:

    bool preparationDone;
   struct{
        QVariantMap arguments;
        QString result ; // should contain "success" after response is parsed
   } reply;

   struct{
        QVariantMap object;
        QVariantMap arguments;
        QByteArray blob;
   } request;

   QNetworkReply* networkReply;


   public:
    RpcCommand(const char * method, QObject* parent=0):
        QObject(parent)
    {
        request.object["method"]=method;
        preparationDone=false;
    }

    public slots:

    virtual QByteArray make()=0;

    virtual void handleReply(){}


    signals:
    void gotReply();

    /**
     * @brief gotTorrentInfo should be emitted whenever data about a torrent becomes available.
     *
     * @param fields A QVariantMap containing at least the *id* filed.
     */
    void gotTorrentInfo(QVariantMap& fields);

};


class RpcConnection : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl server READ server WRITE setserver NOTIFY serverChanged)

    QUrl m_server;



protected:
    void addHeader(QByteArray name, QByteArray value){
        headers.append(QPair<QByteArray, QByteArray> (name, value));
    }

    QNetworkAccessManager networkManager;
    QList<QPair<QByteArray, QByteArray>> headers;

    const char* contentType;


public:
    explicit RpcConnection(QUrl server, QObject *parent = 0);



QUrl server() const
{
    return m_server;
}

signals:

void serverChanged(QUrl arg);

public slots:

void addBasicAuthorisation(QString username, QString password);

virtual void sendCommand(RpcCommand* command){
        QNetworkRequest request ;
        request.setUrl(server());

        request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);


        for(auto header : headers)
            request.setRawHeader(header.first, header.second);

        networkManager.post(request,  command->make());

}

virtual void gotReply(QNetworkReply* reply)=0;

void setserver(QUrl arg)
{
    if (m_server != arg) {
        m_server = arg;
        emit serverChanged(arg);
    }
}

};

#endif // RPCCONNECTION_H
