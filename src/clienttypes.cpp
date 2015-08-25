#include "clienttypes.h"
#include "torrentclient.h"

#include "transmission.h"
#include "rtorrent.h"


namespace ClientTypes {

const QStringList ClientTypes = { "transmission", "rtorrent" };

TorrentClient* makeTorrentClient(QString type, QString name, QString url, QString username, QString password, QVariantMap extraSettings){
    if(type=="transmission")
        return new Transmission(name, url, username, password);
    if(type=="rtorrent")
        return new RTorrent(name, url, username, password);




    qWarning() << "TorrentClientType not known: " << type;
    return nullptr;
}

} // namespace ClientTypes
