#include "rtorrent.h"
#include "torrent.h"
#include "rtorrentCommands/updatecommand.h"

using namespace rtorrentCommands;

RTorrent::RTorrent(QString name, QString url, QString username, QString password, QObject *parent) :
    TorrentClient(name, url, username, password, parent),
    connection(url, this)
{
    
}

void RTorrent::disconnectFromServer()
{
}

void RTorrent::updateTorrents(const QVariantList& torrents, const QList<Field> &fields )
{
    UpdateCommand* command = new UpdateCommand(this);

    for(auto field : fields){
        command->addField(getFieldName(field));
    }

}

void RTorrent::onTorrentData(QVariantMap &data){
    Torrent* t = getTorrent(data["hash"].toString());
    t->updateFields(data);
}

QStringList RTorrent::getFieldName(TorrentClient::Field field)
{
    QStringList result;
    switch(field){
    case ID:
    case HASH:
        result << "hash";
        break;
    case NAME:
        result << "name";
        break;
    case STATE:
        result << "state";
        break;
    case UPSPEED:
        result << "up_rate";
        break;
    case DOWNSPEED:
        result << "down_rate";
        break;

    }
    return result;
}
