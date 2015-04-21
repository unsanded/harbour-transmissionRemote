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

    friend class RpcConnection;
protected:

    bool preparationDone;
   struct{
        QJsonObject arguments;
        QString result ; // should contain "success" after response is parsed
   } reply;

   struct{
        QJsonObject object;
        QJsonObject arguments;
        QJsonDocument doc;
   } request;

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
            request.object["tag"]      =tag();
            request.object["arguments"]=request.arguments;
            request.doc=QJsonDocument(request.object);
            preparationDone=true;
        }
        return request.doc.toJson();
    }

    virtual void parseReplyJson(const QJsonDocument& json );

    virtual void handleReply(){}

protected:
    void setTag(int arg)
    {
        if (m_tag != arg) {
            m_tag = arg;
            emit tagChanged(arg);
        }
    }

signals:
    void gotReply();

    /**
     * @brief gotTorrentInfo should be emitted whenever data about a torrent becomes available.
     *
     * @param fields A JsonObject containing at least the *id* filed.
     */
    void gotTorrentInfo(QJsonObject& fields);

    //this should never be usefull. it is just here because tag is a Q_property
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
