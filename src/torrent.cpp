#include "torrent.h"
#include "torrentfile.h"
#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>
Torrent::Torrent(TorrentClient *parent) :
    QObject(parent)
{
    setfileCount(0);
}

Torrent::Torrent(const Torrent &other):
    QObject((TorrentClient*) other.parent())
{
    m_fileCount=other.m_fileCount;
    setParent(other.parent());
    setid(other.id());
    setname(other.name());
}

TorrentFile* Torrent::addFile(QString filename){

    TorrentFile* file = new TorrentFile(this);
    fileLookup.insert(filename, file);
    fileList.append(file);
    return file;
}

QString Torrent::name() const
{
    return m_name;
}

qreal Torrent::percentage() const
{
    return m_percentage;
}

int Torrent::fileCount() const
{
    return m_fileCount;
}

QString Torrent::id() const
{
    return m_id;
}

int Torrent::upSpeed() const
{
    return m_upSpeed;
}

int Torrent::downSpeed() const
{
    return m_downSpeed;
}


void Torrent::fullUpdate()
{
    QVariantList list;
    list << id().trimmed().toInt();
    QList<TorrentClient::Field> fields = ((TorrentClient*) parent())->getAllTorrentFields();
    fields.append( TorrentClient::FILES);

    ((TorrentClient*) parent())->updateTorrents(list, fields);

}


int Torrent::totalSize() const
{
    return m_totalSize;
}
