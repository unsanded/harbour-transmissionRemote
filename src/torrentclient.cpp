#include "torrentclient.h"
#include "torrent.h"
#include <QDebug>
#include <QList>


const QStringList TorrentClient::saveLocations() const
{
    return m_saveLocations.toList();
}

bool TorrentClient::addSaveLocation(QString location){
    auto iter = m_saveLocations.insert(location);
    if(iter == m_saveLocations.end()){
        return false;
    }
    qDebug() << "adding location " << location;
    emit saveLocationsChanged(saveLocations());
    return true;
}

QQmlListProperty<Torrent> TorrentClient::torrents()
{
    return QQmlListProperty<Torrent>((QObject*) this,  torrentList);
}

bool TorrentClient::connected() const
{
    return m_connected;
}

int TorrentClient::downSpeed() const
{
    return m_downSpeed;
}




void TorrentClient::setUpSpeed(int arg)
{
    if (m_upSpeed != arg) {
        m_upSpeed = arg;
        emit upSpeedChanged(arg);
    }
}

void TorrentClient::setDownSpeed(int arg)
{
    if (m_downSpeed != arg) {
        m_downSpeed = arg;
        emit downSpeedChanged(arg);
    }
}

int TorrentClient::upSpeed() const
{
    return m_upSpeed;
}
