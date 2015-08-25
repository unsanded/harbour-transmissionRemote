#ifndef RPCCOMMANDS_TORRENTGET_H
#define RPCCOMMANDS_TORRENTGET_H
#include "../jsonrpcconnection.h"

#include <QJsonArray>

namespace transmissionCommands {

class TorrentGet : public JsonRpcCommand
{
    Q_OBJECT
    QJsonArray torrentIds;
    QJsonArray fields;
public:
    explicit TorrentGet(QVariantList ids =  QVariantList(), const QStringList& f = QStringList(), QObject *parent = 0);
    virtual void handleReply();

signals:

public slots:

};

} // namespace RpcCommands

#endif // RPCCOMMANDS_TORRENTGET_H
