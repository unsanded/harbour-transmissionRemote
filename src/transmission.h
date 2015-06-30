#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include "torrentclient.h"
#include "jsonrpcconnection.h"


class Transmission : public TorrentClient
{
    Q_OBJECT


    QMap<int, Torrent*> torrentLookup;
public:
    explicit Transmission(QObject *parent = 0);


    RpcConnection* connection;

    virtual Torrent* getTorrent(const QVariant id) const;

public slots:


    bool connectToServer();

    void updateStats();

    void uploadTorrent(QString filename, bool start=true, QString location="");

    void onTorrentData(QVariantMap &data);
    void onUpdateDone();


    void setServer(QUrl arg);

    // TorrentClient interface
public slots:
    virtual void updateTorrents(const QVariantList& torrents = QVariantList(), const QStringList& fields = QStringList());
};

#endif // TRANSMISSION_H
