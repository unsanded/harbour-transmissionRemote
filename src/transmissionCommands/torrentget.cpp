#include "torrentget.h"

namespace transmissionCommands {

TorrentGet::TorrentGet(QVariantList ids, const QStringList& f, QObject *parent) :
    JsonRpcCommand("torrent-get", parent),
    torrentIds(QJsonArray::fromVariantList(ids)),
    fields(    QJsonArray::fromStringList(f))
{
    if(!fields.contains(QString("id")))
        fields.append(  QString("id"));
    if(!fields.contains(QString("name")))
        fields.append(  QString("name"));
    if(!torrentIds.empty())
    {
        requestArguments["ids"] = torrentIds;
    }
    qDebug() << "updating fields" << fields;
    requestArguments["fields"] = fields;

}

void TorrentGet::handleReply()
{
    QVariantList torrents = replyArguments["torrents"].toList() ;

    for(QVariantList::iterator iter=torrents.begin(); iter!=torrents.end(); iter++){
        QVariantMap torrent = (*iter).toMap();
        emit gotTorrentInfo(torrent);
    }



}

} // namespace RpcCommands
