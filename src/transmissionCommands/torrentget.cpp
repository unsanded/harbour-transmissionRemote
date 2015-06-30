#include "torrentget.h"
#include <QJsonArray>
#include <QJsonObject>

namespace transmissionCommands {

TorrentGet::TorrentGet(QVariantList ids, const QStringList& f, QObject *parent) :
    JsonRpcCommand("torrent-get", parent),
    torrentIds(ids),
    fields(f)
{
    if(!fields.contains("id"))
        fields << "id";
    if(!fields.contains("name"))
        fields << "name";
    if(!fields.contains("percentDone"))
        fields << "percentDone";
    if(!fields.contains("eta"))
        fields << "eta";
    if(!fields.contains("downloadDir"))
        fields << "downloadDir";
    if(!torrentIds.empty())
    {
        request.arguments["ids"] = QJsonArray::fromVariantList(torrentIds);
    }
    request.arguments["fields"] = fields;

}

void TorrentGet::handleReply()
{
    QVariantList torrents = reply.arguments["torrents"].toList() ;

    for(QVariantList::iterator iter=torrents.begin(); iter!=torrents.end(); iter++){
        QVariantMap torrent = (*iter).toMap();
        emit gotTorrentInfo(torrent);
    }



}

} // namespace RpcCommands
