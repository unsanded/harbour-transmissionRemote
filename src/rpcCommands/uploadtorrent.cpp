#include "uploadtorrent.h"

#include <QFile>

namespace RpcCommands {

uploadTorrent::uploadTorrent(QString torrentFile, QObject *parent) :
    RpcCommand("torrent-add", parent)
{
    QFile file(torrentFile);

    if(!file.exists()){
        qWarning() << "adding non-existing torrent file";
        return;
    }
    file.open(QFile::ReadOnly);

    if(!file.isOpen())
    {
        QByteArray data = file.readAll();
        request.arguments["metainfo"] = QString(data.toBase64());
    }
}

} // namespace RpcCommands




void RpcCommands::uploadTorrent::handleReply()
{
    if(reply.result=="success"){
        emit gotTorrentInfo(reply.arguments);
    }
}


