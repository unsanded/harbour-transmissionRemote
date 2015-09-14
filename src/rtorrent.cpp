#include "rtorrent.h"
#include "torrent.h"
#include "rtorrenttorrent.h"
#include "rtorrentCommands/updatecommand.h"
#include "rtorrentCommands/addtorrent.h"

using namespace rtorrentCommands;

RTorrent::RTorrent(QString name, QString url, QString username, QString password, QObject *parent) :
    TorrentClient(name, url, username, password, parent),
    connection(url, this)
{
    
}

void RTorrent::uploadTorrent(const QString &filename, bool autostart, QString downloadDir){

    auto command = new addTorrentCommand (filename, downloadDir);
    command->setAutoStart(autostart);

    connect(command, &RpcCommand::done,
            command, &QObject::deleteLater);

    connection.sendCommand(command);


}

void RTorrent::disconnectFromServer()
{
}

void RTorrent::updateTorrents(const QVariantList& torrents, const QList<Field> &fields )
{
    UpdateCommand* command = new UpdateCommand(this);
    if(fields.empty()){
        QList<Field> defaultFields;
        defaultFields << HASH  << NAME <<  DOWNSPEED << UPSPEED << DOWNLOADED << DOWNLOADDIR;

        for(auto field : defaultFields){
            command->addField(getFieldName(field));
        }
    }
    else
    for(auto field : fields){
        command->addField(getFieldName(field));
    }
    connect(
        command, &RpcCommand::gotTorrentInfo,
        this,    &RTorrent::onTorrentData
       );

    //delete, as soon as we got it's data
    connect(
            command, &RpcCommand::gotTorrentInfo,
            command, &QObject::deleteLater
           );

    connection.sendCommand(command);
}

void RTorrent::onTorrentData(QVariantMap &data){
    QString hash = data["hash"].toString();
    Torrent* t = getTorrent(hash);
    if(t == nullptr){
        qDebug() << "new Torrent";
        t  = new RTorrentTorrent(this);
        addTorrent(t, hash);
    }
    t->updateFields(data);
}

QStringList RTorrent::getFieldName(TorrentClient::Field field)
{
    QStringList result;
    switch(field){
    default:
        result << "name";
    case ID:
    case HASH:
        result << "hash";
        break;
    case NAME:
        result << "name";
        break;
    case STATE:
        result << "state" << "is_hash_checking";
        break;
    case UPSPEED:
        result << "up_rate";
        break;
    case DOWNSPEED:
        result << "down_rate";
        break;
    case STATUS:
        result << "message";
        break;
    case SIZE:
        result << "size_bytes";
        break;
    case DOWNLOADED:
        result << "completed_bytes";
        break;
    case UPLOADED:
        result << "up_total";
        break;
    case SEEDS:
        result << "peers_completed";
        break;
    case PEERS:
        result << "peers_accounted";
        break;
    case DOWNLOADDIR:
        result << "directory_base";
        break;
    }
    return result;
}
