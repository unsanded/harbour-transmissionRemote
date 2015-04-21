#ifndef RPCCOMMANDS_TORRENTGET_H
#define RPCCOMMANDS_TORRENTGET_H
#include "../rpcconnection.h"
#include <QJsonObject>

namespace RpcCommands {

class TorrentGet : public RpcCommand
{
    Q_OBJECT
    QList<int> torrentIds;
    QStringList fields;
public:
    explicit TorrentGet(QList<int> ids = QList<int>(), QStringList fields = QStringList(), QObject *parent = 0);
    virtual void handleReply();

signals:

public slots:

};

} // namespace RpcCommands

#endif // RPCCOMMANDS_TORRENTGET_H
