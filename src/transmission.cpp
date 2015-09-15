#include "transmission.h"
#include "transmissionCommands/sessionstats.h"
#include "transmissionCommands/torrentget.h"
#include "transmissionCommands/uploadtorrent.h"
#include "rpcconnection.h"
#include "torrent.h"
#include "transmissiontorrent.h"

#include <QtAlgorithms>

using namespace transmissionCommands;

Transmission::Transmission(QString name, QString url, QString username, QString password, QObject *parent) :
    TorrentClient(name, url, username, password, parent)
{
    connectToServer();
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

void Transmission::disconnectFromServer()
{
    m_connected = false;
    emit connectedChanged(false);
    delete connection;
    connection = 0;
}


void Transmission::updateTorrents(const QVariantList& ids, const QList<int> &fields)
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

void Transmission::uploadTorrent(const QString &filename, bool start, QString location)
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
    QString id=QString("%1").arg(data["id"].toInt(), 3, 10);

    Torrent* t= getTorrent(id);
    if(!t){
        t=new TransmissionTorrent(this);
        t->setid(id);
        addTorrent(t, id);
        connect(t, SIGNAL(downloadDirChanged(QString)),
                this, SLOT(addSaveLocation(QString)));

        if(data.contains("downloadDir"))
        {
            QString dir = data["downloadDir"].toString();
            addSaveLocation(dir);
        }
        emit torrentsChanged(torrents());
    }
    t->updateFields(data);
}

void Transmission::onUpdateDone()
{
}

QList<TorrentClient::Field> Transmission::getAllTorrentFields() const
{
    QList<TorrentClient::Field> result;
    for(int i = 0; i<=RATIOLIMIT; i++)
        result.append( (Field)i );
    return result;
}
