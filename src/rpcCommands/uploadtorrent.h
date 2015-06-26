#ifndef RPCCOMMANDS_UPLOADTORRENT_H
#define RPCCOMMANDS_UPLOADTORRENT_H
#include "../rpcconnection.h"

#include <QFile>

namespace RpcCommands {
/**
 * @brief The uploadTorrent class
 * RPC command to upload a local torrent file
 */
class UploadTorrent : public RpcCommand
{
    Q_OBJECT
    Q_PROPERTY(bool autoStart READ autoStart WRITE setAutoStart NOTIFY autoStartChanged)
    Q_PROPERTY(QString downloadDir READ downloadDir WRITE setDownloadDir NOTIFY downloadDirChanged)

    bool m_autoStart;
    QString m_downloadDir;

public:
    explicit UploadTorrent(QString torrentFile, bool autostart=true, QString downloadDir="", QObject *parent = 0);

bool autoStart() const
{
    return m_autoStart;
}

QString downloadDir() const
{
    return m_downloadDir;
}

signals:

    void torrentAdded(QJsonObject& torrentInfo);
    void torrentAddFailed(QJsonObject& failureInfo);

    //auto-generated stuff
    void autoStartChanged(bool arg);
    void downloadDirChanged(QString arg);

public slots:

    // RpcCommand interface
public slots:
virtual void handleReply();


    //auto-generated stuff
void setAutoStart(bool arg)
{
    if (m_autoStart != arg) {
        m_autoStart = arg;
        request.arguments["paused"] = !m_autoStart;
        emit autoStartChanged(arg);
    }
}
void setDownloadDir(QString arg)
{
    if (m_downloadDir != arg) {
        m_downloadDir = arg;
        request.arguments["download-dir"] = m_downloadDir;
        emit downloadDirChanged(arg);
    }
}
};

} // namespace RpcCommands

#endif // RPCCOMMANDS_UPLOADTORRENT_H
