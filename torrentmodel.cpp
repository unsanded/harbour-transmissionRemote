#include "torrentmodel.h"
#include "rpcconnection.h"
TorrentModel::TorrentModel(RpcConnection* connection, QObject *parent) :
    QAbstractListModel(parent)
{
}

TorrentModel::TorrentModel(QObject *parent):
    QAbstractListModel(parent)
{

}


int TorrentModel::rowCount(QModelIndex &parent)
{
    return torrents.size();
}

int TorrentModel::columnCount(QModelIndex &parent)
{
    return 1;
}

QVariant TorrentModel::data(QModelIndex &index, int role)
{
    Torrent& torrent = torrents[index.row()];
    switch (role) {
    case torrentIdRole:
        return torrent.id();
        break;
    case percentageRole:
        return torrent.percentage();
    default:
        break;
    }
}


QHash<int, QByteArray> TorrentModel::roleNames()
{
    QHash<int, QByteArray> result;
    result[torrentIdRole]  ="torrentId";
    result[torrentNameRole]="name";
    result[percentageRole] ="percentage";
}
