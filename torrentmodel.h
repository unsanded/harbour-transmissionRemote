#ifndef TORRENTMODEL_H
#define TORRENTMODEL_H

#include <QAbstractListModel>
#include "rpcconnection.h"
#include "torrent.h"

class TorrentModel : public QAbstractListModel
{
    Q_OBJECT


    QVector<Torrent> torrents;

public:
    explicit TorrentModel(RpcConnection* connection, QObject *parent = 0);
    explicit TorrentModel( QObject *parent = 0);

signals:

public slots:


    // QAbstractItemModel interface
public:
    enum TorrentFields{
        torrentIdRole=Qt::UserRole,
        torrentNameRole=Qt::UserRole+1,
        percentageRole =Qt::UserRole+2,
        filecountRole  =Qt::UserRole+3,


    };
    virtual int rowCount(QModelIndex &parent);
    virtual int columnCount(QModelIndex &parent);
    virtual QVariant data(QModelIndex &index, int role);

    // QAbstractItemModel interface

    // QAbstractItemModel interface
public:
    virtual QHash<int, QByteArray> roleNames();
};

#endif // TORRENTMODEL_H










