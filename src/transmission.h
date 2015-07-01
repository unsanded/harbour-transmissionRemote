#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include "torrentclient.h"
#include "jsonrpcconnection.h"


class Transmission : public TorrentClient
{
    Q_OBJECT


    QMap<int, Torrent*> torrentLookup;
public:
    explicit Transmission(QString name, QString url, QString username=QString(), QString password=QString(), QObject *parent = 0);


    RpcConnection* connection;

    Q_INVOKABLE virtual Torrent* getTorrent(int id) const;

    Q_INVOKABLE virtual QStringList getAllTorrentFields() const;

public slots:


    bool connectToServer();

    void updateStats();

    void uploadTorrent(QString filename, bool start=true, QString location="");

    void onTorrentData(QVariantMap &data);
    void onUpdateDone();



    // TorrentClient interface
public slots:
    virtual void updateTorrents(const QVariantList& torrents = QVariantList(), const QStringList& fields = QStringList());
    virtual const char* clientType(){
        return "transmission";
    }
};

#endif // TRANSMISSION_H
