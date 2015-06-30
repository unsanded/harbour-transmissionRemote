#include "uploadtorrent.h"
#include <QFile>

namespace transmissionCommands {

UploadTorrent::UploadTorrent(QString torrentFile, bool autostart, QString downloadDir, QObject *parent) :
    JsonRpcCommand("torrent-add", parent)
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
        qWarning() << "unable to open torrent  file";


    setAutoStart(autostart);
    if(!downloadDir.isEmpty())
        setDownloadDir(downloadDir);


}

void UploadTorrent::handleReply()
{
    if(reply.result=="success"){
        qDebug() << "succesfully uploaded torrent";
        emit gotTorrentInfo(reply.arguments);
    }
    else
      qWarning() << "torrent Upload failed " << reply.result
                                     << reply.arguments;
}

} // namespace transmissionCommands

