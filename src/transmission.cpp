#include "transmission.h"
#include "rpcCommands/torrentget.h"
#include "rpcconnection.h"

using namespace RpcCommands;

Transmission::Transmission(QObject *parent) :
    QObject(parent)
{

    connection=NULL;
}

void Transmission::update()
{
    TorrentGet* getCommand= new TorrentGet;
    connect(
                getCommand, SIGNAL(gotTorrentInfo(int, QJsonObject)),
                this, SLOT(onTorrentData(int, QJsonObject))
            );
            connection->sendCommand(getCommand);

    emit torrentsChanged(torrents());
}

void Transmission::onTorrentData(int id, QJsonObject data)
{
    Torrent* t= torrentLookup[id];
    if(!t){
        t=new Torrent(this);
        t->setid(id);
        torrentLookup.insert(id, t);
        torrentList.append(t);
        emit torrentsChanged(torrents());
    }
    t->updateFields(data);




}



