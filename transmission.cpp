#include "transmission.h"

Transmission::Transmission(QObject *parent) :
    QObject(parent)

{



    Torrent *t = new Torrent(this);
    t->setfileCount(44);
    t->setname("modelT11");
    torrentList.append(t);
    emit torrentsChanged(torrents());
}

void Transmission::update()
{
    Torrent *t = new Torrent(this);
    t->setfileCount(44);
    t->setname("modelT11");
    torrentList.append(t);
    emit torrentsChanged(torrents());
}
