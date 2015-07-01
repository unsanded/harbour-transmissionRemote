#include "clienttypes.h"
#include "torrentclient.h"

#include "transmission.h"


namespace ClientTypes {

const QStringList ClientTypes = { "transmission" };

TorrentClient* makeTorrentClient(QString type, QString name, QString url, QString username, QString password, QVariantMap extraSettings){
    if(type=="transmission")
        return new Transmission(name, url, username, password);




    qWarning() << "TorrentClientType not known: " << type;
    return nullptr;
}

} // namespace ClientTypes
