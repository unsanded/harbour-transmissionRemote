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
    for(QJsonObject::iterator iter=freshData.begin(); iter!=freshData.end(); iter++){
        if(iter.value().isString())
            fields.insert(iter.key(), iter.value().toString());
        if(iter.value().isDouble())
            fields.insert(iter.key(), iter.value().toDouble());
        if(iter.value().isBool())
            fields.insert(iter.key(), iter.value().toBool());
    }
    setname(freshData["name"].toString());
    setpercentage(freshData["percentDone"].toDouble() * 100.0);


}
