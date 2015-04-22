#include "transmission.h"
#include "rpcCommands/torrentget.h"
#include "rpcCommands/uploadtorrent.h"
#include "rpcCommands/sessionstats.h"
#include "rpcconnection.h"

using namespace RpcCommands;

Transmission::Transmission(QObject *parent) :
    QObject(parent)
{

    m_upSpeed=0;
    m_downSpeed=0;
    connection=NULL;
}

Torrent *Transmission::getTorrent(int id)
{
    return torrentLookup[id];
}

void Transmission::update()
{
    TorrentGet* getCommand= new TorrentGet;
    connect(
                getCommand, SIGNAL(gotTorrentInfo(QJsonObject& )),
                this, SLOT(onTorrentData( QJsonObject& ))
            );
    connect(
                getCommand, SIGNAL(gotReply()),
                this      , SLOT(onUpdateDone())
            );
    connection->sendCommand(getCommand);
}

void Transmission::updateStats()
{
    SessionStats*  command = new SessionStats(this);
    connect(
              command, SIGNAL(gotUpspeed(int)),
                this, SLOT(setUpSpeed(int))
            );

    connect(
              command, SIGNAL(gotDownSpeed(int)),
                this, SLOT(setDownSpeed(int))
            );

    connection->sendCommand(command);
}

void Transmission::uploadTorrent(QString filename)
{
    RpcCommands::uploadTorrent* command = new RpcCommands::uploadTorrent(filename, this);
    connect(
                command, SIGNAL(gotTorrentInfo(QJsonObject&)),
                this,       SLOT(onTorrentData(QJsonObject&))
           );

    connection->sendCommand(command);

}

void Transmission::onTorrentData(QJsonObject& data)
{
    int id=data["id"].toInt();
    Torrent* t= torrentLookup[id];
    if(!t){
        qDebug() << "new torent " <<  id;
        t=new Torrent(connection, this);
        t->setid(id);
        torrentLookup.insert(id, t);
        torrentList.append(t);
        emit torrentsChanged(torrents());
    }
    t->updateFields(data);
}

void Transmission::onUpdateDone()
{
}



