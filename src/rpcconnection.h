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

   QNetworkReply* networkReply;
   bool preparationDone;

   QString result ; // should contain "success" after response is parsed

    QString method;
    QVariantMap object;

    QByteArray requestBlob;



   public:
    RpcCommand(const char * method, QObject* parent=0):
        QObject(parent),
        method(method)
    {

        preparationDone=false;
    }

    public slots:

    virtual QByteArray make() =0;

    virtual void gotReply()   =0;
    virtual void handleReply()=0;


    signals:

    /**
     * @brief gotTorrentInfo should be emitted whenever data about a torrent becomes available.
     *
     * @param fields A QVariantMap containing at least the *id* filed.
     */
    void gotTorrentInfo(QVariantMap& fields);
    /**
     * @brief done should be emitted when the command has fulfilled it's purpose.
     * Usualy this means that it can be delete. eg. by connecting this signal to `deleteLater()`.
     */
    void done();

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

virtual void sendCommand(RpcCommand* command);

void setserver(QUrl arg);

};

#endif // RPCCONNECTION_H
