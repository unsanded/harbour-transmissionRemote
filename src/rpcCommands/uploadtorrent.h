#ifndef RPCCOMMANDS_UPLOADTORRENT_H
#define RPCCOMMANDS_UPLOADTORRENT_H
#include "../rpcconnection.h"

#include <QFile>

namespace RpcCommands {
/**
 * @brief The uploadTorrent class
 * RPC command to upload a local torrent file
 */
class uploadTorrent : public RpcCommand
{
    Q_OBJECT
public:
    explicit uploadTorrent(QString torrentFile, QObject *parent = 0);

signals:

    void torrentAdded(QJsonObject& torrentInfo);
    void torrentAddFailed(QJsonObject& failureInfo);
public slots:


    // RpcCommand interface
public slots:
    virtual void handleReply();
};

} // namespace RpcCommands

#endif // RPCCOMMANDS_UPLOADTORRENT_H
