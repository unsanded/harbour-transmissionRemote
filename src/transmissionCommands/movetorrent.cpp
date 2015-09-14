#include "movetorrent.h"

#include <src/jsonrpcconnection.h>

#include <QJsonArray>

namespace transmissionCommands {

moveTorrent::moveTorrent(Torrent* torrent, QString destination, QObject *parent) :
    JsonRpcCommand("torrent-set-location", parent)
{
    requestArguments["move"] = true;
    QJsonArray ids;
    ids.append(torrent->id());
    requestArguments["ids"]=ids;
    requestArguments["location"] = destination;
}


} // namespace transmissionCommands
