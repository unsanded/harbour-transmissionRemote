#include "torrent.h"
#include <QJsonObject>

#include <QDebug>
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


void Torrent::updateFields(QJsonObject &freshData){

    qDebug() << "updating fields";

    if(freshData.contains("name"))
        setname(freshData["name"].toString());
    if(freshData.contains("percentDone"))
        setpercentage(freshData["percentDone"].toDouble() * 100.0);


}
