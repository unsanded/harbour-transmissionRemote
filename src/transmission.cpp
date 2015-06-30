#include "transmission.h"
#include "transmissionCommands/sessionstats.h"
#include "transmissionCommands/torrentget.h"
#include "transmissionCommands/uploadtorrent.h"
#include "rpcconnection.h"
#include "torrent.h"

#include <QtAlgorithms>

using namespace transmissionCommands;

Transmission::Transmission(QObject *parent) :
    TorrentClient(parent)
{
    connection=nullptr;
}

Torrent* Transmission::getTorrent(const QVariant id) const
{
    return torrentLookup[id.toInt()];
}

bool Transmission::connectToServer()
{
    connection = new JsonRpcConnection(server(), this);
    return true;
}


void Transmission::updateTorrents(const QVariantList& ids, const QStringList& fields)
{

    TorrentGet* getCommand= new TorrentGet(ids, fields);
    connect(
                getCommand, SIGNAL(gotTorrentInfo(QVariantMap&)),
                this, SLOT(onTorrentData(QVariantMap&) )
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

void Transmission::uploadTorrent(QString filename, bool start, QString location)
{
    transmissionCommands::UploadTorrent* command = new transmissionCommands::UploadTorrent(filename, start, location, this);

    connect(
                command, SIGNAL(gotTorrentInfo(QVariantMap&)),
                this,       SLOT(onTorrentData(QVariantMap&))
           );

    connection->sendCommand(command);

}

void Transmission::onTorrentData(QVariantMap& data)
{
    int id=data["id"].toInt();
    Torrent* t= torrentLookup[id];
    if(!t){
        qDebug() << "new torent " <<  id;
        t=new Torrent(this);
        t->setid(id);
        torrentLookup.insert(id, t);
        torrentList.append(t);
        if(data.contains("downloadDir"))
        {
            QString dir = data["downloadDir"].toString();
            //go to parent directory
            if(dir[1]==':')
                dir.truncate(dir.lastIndexOf('\\', -2));
            else
                dir.truncate(dir.lastIndexOf('/', -2));
            addSaveLocation(dir);
        }
        emit torrentsChanged(torrents());
    }
    t->updateFields(data);
}

void Transmission::onUpdateDone()
{
}

void Transmission::setServer(QUrl arg)
{
    if (!connection){
        connection=new JsonRpcConnection(arg, this);
        emit serverChanged(arg);
    }
    else if (connection->server() != arg)
    {
        connection->setserver(arg);
        emit serverChanged(arg);
    }
}




