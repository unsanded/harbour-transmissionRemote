#include "uploadtorrent.h"

#include <QFile>

namespace RpcCommands {

uploadTorrent::uploadTorrent(QString torrentFile, QObject *parent) :
    RpcCommand("torrent-add", parent)
{
    qDebug() << "uploading torrent file: " << torrentFile;
    QFile file(torrentFile);

    if(!file.exists()){
        qWarning() << "adding non-existing torrent file";
        return;
    }
    file.open(QFile::ReadOnly);

    if(file.isOpen())
    {
        QByteArray data = file.readAll();
        request.arguments["metainfo"] = QString(data.toBase64());
    }
    else
        qWarning() << "torrentfile not open";
}

} // namespace RpcCommands




void RpcCommands::uploadTorrent::handleReply()
{
    if(reply.result=="success"){
        qDebug() << "succesfully uploaded torrent";
        emit gotTorrentInfo(reply.arguments);
    }
    else
      qWarning() << "torrent Upload failed " << reply.result
                                     << reply.arguments;
}


