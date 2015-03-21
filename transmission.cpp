#include "transmission.h"

Transmission::Transmission(QObject *parent) :
    QObject(parent)

{



    Torrent* t=new Torrent(this);
    t->setfileCount(42);
    t->setname("modelT");
    torrentList.append(t);
    emit torrentsChanged(torrents());
}
