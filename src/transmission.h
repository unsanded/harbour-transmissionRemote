#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include "torrentclient.h"
#include "jsonrpcconnection.h"
#include "torrent.h"


class Transmission : public TorrentClient
{
    Q_OBJECT


public:
    explicit Transmission(QString name, QString url, QString username=QString(), QString password=QString(), QObject *parent = 0);




    Q_INVOKABLE virtual QList<Field> getAllTorrentFields() const;


    static QStringList getFieldName(Field field){
        QStringList res;
        switch(field){
        case HASH:
            res << "hashString";
        case ID:
            res <<  "id";
            break;
        case NAME:
            res <<  "name";
            break;
        case PERCENTAGE:
            res << "percentDone";
            break;
        case DOWNLOADDIR:
            res <<  "downloadDir";
            break;
        case FILES:
            res << "files";
            res <<  "filesstats";
            res << "wanted";
            break;
        case FILECOUNT:
             res <<  "files";
             break;
        case DOWNLOADED:
             res <<  "totalSize";
             break;
        case UPLOADED:
             res <<  "uploadedEver";
             break;
        case SIZE:
             res <<  "sizeWhenDone";
             break;
         case PEERS:
             res <<  "peersConnected";
             break;
        case PEERSGETTING:
            res << "peersGettingFromUs";
            break;
        case PEERSGIVING:
            res <<  "peersGivingToUs";
            break;
        case ETA:
            res << "eta";
            break;
        case SEEDS:
            res << "peers";
            break;
        case DOWNSPEED:
            res << "rateDownload";
            break;
        case UPSPEED:
            res << "rateUpload";
            break;
        case STATUS:
            res << "errorString";
            res << "error";
            break;
        default:
            qWarning() << "unimplemented Field " << field;
        }
        return res;
    }

public slots:


    bool connectToServer();
    virtual void disconnectFromServer();

    void updateStats();

    virtual void uploadTorrent(const QString& filename, bool start=true, QString location="");

    void onUpdateDone();


    // TorrentClient interface
    virtual void updateTorrents(const QVariantList& torrents = QVariantList(), const QList<int>& fields = QList<int>());

    virtual const char* clientType(){
        return "transmission";
    }

    void onTorrentData(QVariantMap &data);

    friend class TransmissionTorrent;

protected:

    RpcConnection* connection;
};

#endif // TRANSMISSION_H
