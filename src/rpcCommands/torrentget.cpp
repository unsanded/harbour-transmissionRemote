#include "torrentget.h"
#include <QJsonArray>
#include <QJsonObject>

namespace RpcCommands {

TorrentGet::TorrentGet(QList<int> ids, QStringList fields, QObject *parent) :
    RpcCommand("torrent-get", parent),
    torrentIds(ids),
    fields(fields)
{
    if(!fields.contains("id"))
        fields << "id";
    if(!fields.contains("name"))
        fields << "name";
    if(!fields.contains("percentDone"))
        fields << "percentDone";

    qDebug() << fields;
    if(!torrentIds.empty())
    {
        QVariantList torrIds;
        for (int id : torrentIds)
            torrIds.append(QVariant(id));
        request.arguments["ids"] = QJsonArray::fromVariantList(torrIds);
    }
    request.arguments["fields"] = QJsonArray::fromStringList(fields);

}

void TorrentGet::handleReply()
{
    QJsonArray torrents = reply.arguments["torrents"].toArray() ;

    for(QJsonArray::iterator iter=torrents.begin(); iter!=torrents.end(); iter++){
        QJsonObject torrent = (*iter).toObject();
        emit gotTorrentInfo(torrent["id"].toInt(), torrent);
    }
}

} // namespace RpcCommands
