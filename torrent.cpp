#include "torrent.h"

Torrent::Torrent(QObject *parent) :
    QObject(parent)
{
    setfileCount(1);
}

Torrent::Torrent(const Torrent &other)
{
    m_fileCount=other.m_fileCount;
    setParent(other.parent());
    setid(other.id());
    setname(other.name());
}
