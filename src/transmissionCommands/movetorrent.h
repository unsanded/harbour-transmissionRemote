#ifndef TRANSMISSIONCOMMANDS_MOVETORRENT_H
#define TRANSMISSIONCOMMANDS_MOVETORRENT_H
#include <src/jsonrpcconnection.h>
#include <src/torrent.h>

namespace transmissionCommands {

class moveTorrent : public JsonRpcCommand
{
    Q_OBJECT
public:
    explicit moveTorrent(Torrent *torrent, QString destination, QObject *parent = 0);

    void  handleReply(){

    }

signals:

public slots:

};

} // namespace transmissionCommands

#endif // TRANSMISSIONCOMMANDS_MOVETORRENT_H
