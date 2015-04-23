#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include "torrent.h"
#include "rpcconnection.h"

#include <QObject>
#include <QQmlListProperty>
#include <QFile>

class Transmission : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Torrent> torrents READ torrents  NOTIFY torrentsChanged)
    Q_PROPERTY(QUrl server READ server WRITE setServer NOTIFY serverChanged)
    Q_PROPERTY(int upSpeed READ upSpeed WRITE setUpSpeed NOTIFY upSpeedChanged)
    Q_PROPERTY(int downSpeed READ downSpeed WRITE setDownSpeed NOTIFY downSpeedChanged)

    QList<Torrent*> torrentList;
    QMap<int, Torrent*> torrentLookup;
    RpcConnection* connection;

    int m_upSpeed;

    int m_downSpeed;

public:
    explicit Transmission(QObject *parent = 0);


QQmlListProperty<Torrent> torrents()
{
    return QQmlListProperty<Torrent>((QObject*) this,  torrentList);
}

QUrl server() const
{
    if(connection)
        return connection->server();
    else
        return QUrl();
}

int upSpeed() const
{
    return m_upSpeed;
}

int downSpeed() const
{
    return m_downSpeed;
}

signals:

    void torrentsChanged(QQmlListProperty<Torrent> arg);

    void serverChanged(QUrl arg);

    void upSpeedChanged(int arg);

    void downSpeedChanged(int arg);

public slots:

    Torrent* getTorrent(int id);

    void update();
    void updateStats();

    void uploadTorrent(QString filename);

    void onTorrentData( QJsonObject& data);
    void onUpdateDone();
    void setServer(QUrl arg)
    {
        if (!connection){
            connection=new RpcConnection(arg, this);
            emit serverChanged(arg);
        }
        else if (connection->server() != arg)
        {
            connection->setserver(arg);
            emit serverChanged(arg);
        }
    }
    void setUpSpeed(int arg)
    {
        if (m_upSpeed != arg) {
            m_upSpeed = arg;
            emit upSpeedChanged(arg);
        }
    }
    void setDownSpeed(int arg)
    {
        if (m_downSpeed != arg) {
            m_downSpeed = arg;
            emit downSpeedChanged(arg);
        }
    }
};

#endif // TRANSMISSION_H
