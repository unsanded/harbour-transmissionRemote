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
        requestArguments["metainfo"] = QString(data.toBase64());
    }
    else
        qWarning() << "unable to open torrent  file";


    setAutoStart(autostart);
    if(!downloadDir.isEmpty())
        setDownloadDir(downloadDir);


}

void UploadTorrent::handleReply()
{
    if(result == "success"){
        qDebug() << "succesfully uploaded torrent";
        emit gotTorrentInfo(replyArguments);
    }
    else
      qWarning() << "torrent Upload failed " << result
                                     << replyArguments;
}

} // namespace transmissionCommands

