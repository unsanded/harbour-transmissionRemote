#include "transmission.h"
#include "transmissionCommands/sessionstats.h"
#include "transmissionCommands/torrentget.h"
#include "transmissionCommands/uploadtorrent.h"
#include "rpcconnection.h"
#include "torrent.h"

#include <QtAlgorithms>

using namespace transmissionCommands;

Transmission::Transmission(QString name, QString url, QString username, QString password, QObject *parent) :
    TorrentClient(name, url, username, password, parent)
{
    connectToServer();
}

Torrent* Transmission::getTorrent(int id) const
{
    return torrentLookup[id];
}

bool Transmission::connectToServer()
{
    connection = new JsonRpcConnection(url(), this);
    if(!username().isEmpty() || !password().isEmpty()){
        connection->addBasicAuthorisation(username(), password());
    }
    //send some command, just to get a session cookie
    updateStats();
    return true;
}


void Transmission::updateTorrents(const QVariantList& ids, const QList<Field>& fields)
{

    QStringList fieldnames;

    for(auto field: fields)
        fieldnames.append(getFieldName(field));

    TorrentGet* getCommand = new TorrentGet(ids, fieldnames);
    connect(
                getCommand, SIGNAL(gotTorrentInfo(QVariantMap&)),
                this, SLOT(onTorrentData(QVariantMap&) )
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

QStringList Transmission::getAllTorrentFields() const
{
    QStringList fields;
    return fields;
}
