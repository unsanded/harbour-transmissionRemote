#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include "torrent.h"
#include "rpcconnection.h"

#include <QObject>
#include <QQmlListProperty>

class Transmission : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Torrent> torrents READ torrents  NOTIFY torrentsChanged)
    Q_PROPERTY(QUrl server READ server WRITE setServer NOTIFY serverChanged)

    QQmlListProperty<Torrent> m_torrents;
    QList<Torrent*> torrentList;

    QMap<int, Torrent*> torrentLookup;

    RpcConnection* connection;

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

signals:

    void torrentsChanged(QQmlListProperty<Torrent> arg);

    void serverChanged(QUrl arg);

public slots:

    void update();

    void onTorrentData(int id, QJsonObject data);

    void setServer(QUrl arg)
    {
        if (!connection)
            connection=new RpcConnection(arg, this);
        else if (connection->server() != arg)
        {
            connection->setserver(arg);
            emit serverChanged(arg);
        }
    }
};

#endif // TRANSMISSION_H
