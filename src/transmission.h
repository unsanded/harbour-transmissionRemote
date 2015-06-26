#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include "torrent.h"
#include "rpcconnection.h"

#include <QObject>
#include <QQmlListProperty>
#include <QFile>
#include <QDir>

class Transmission : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Torrent> torrents READ torrents  NOTIFY torrentsChanged)
    Q_PROPERTY(QUrl server READ server WRITE setServer NOTIFY serverChanged)
    Q_PROPERTY(int upSpeed READ upSpeed WRITE setUpSpeed NOTIFY upSpeedChanged)
    Q_PROPERTY(int downSpeed READ downSpeed WRITE setDownSpeed NOTIFY downSpeedChanged)
    Q_PROPERTY(QStringList saveLocations READ saveLocations NOTIFY saveLocationsChanged)

    QList<Torrent*> torrentList;
    QMap<int, Torrent*> torrentLookup;
    RpcConnection* connection;

    int m_upSpeed;

    int m_downSpeed;

    QSet<QString> m_saveLocations;

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

QStringList saveLocations() const
{
    return m_saveLocations.toList();
}

signals:

    void torrentsChanged(QQmlListProperty<Torrent> arg);

    void serverChanged(QUrl arg);

    void upSpeedChanged(int arg);

    void downSpeedChanged(int arg);

    void saveLocationsChanged(QStringList arg);

public slots:

    Torrent* getTorrent(int id);

    void update(QStringList fields = QStringList());
    void updateStats();

    void uploadTorrent(QString filename, bool start=true, QString location="");

    void onTorrentData( QJsonObject& data);
    void onUpdateDone();

    bool addSaveLocation(QString location){
        auto iter = m_saveLocations.insert(location);
        if(iter == m_saveLocations.end()){
            return false;
        }
        qDebug() << "adding location " << location;
        emit saveLocationsChanged(saveLocations());
        return true;
    }

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
