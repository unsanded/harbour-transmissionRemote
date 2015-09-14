#include "torrentclient.h"
#include "xmlrpcconnection.h"

#ifndef RTORRENT_H
#define RTORRENT_H

class RTorrent : public TorrentClient
{
    Q_OBJECT

    XmlRpcConnection connection;
public:
    explicit RTorrent(QString name, QString url, QString username="", QString password="", QObject *parent = 0);

signals:

public slots:


    // TorrentClient interface
public:
    virtual QList<Field> getAllTorrentFields() const
    {
        QList<Field> result;

        result << HASH;
        result << FILES;
        result << NAME;
        result << UPLOADED;
        result << DOWNSPEED;
        result << UPSPEED;
        result << DOWNLOADDIR;
        result << DOWNLOADED;
        result << FILES;


        return result;
    }

    virtual const char *clientType()
    {
        return "rtorrent";
    }


public slots:
    virtual void uploadTorrent(const QString& filename, bool autostart=true, QString downloadDir="");

    virtual bool connectToServer()
    {
        return true;
    }
    virtual void disconnectFromServer();

    virtual void updateTorrents(const QVariantList& torrents = QVariantList(), const QList<Field> &fields = QList<Field>());

    void onTorrentData(QVariantMap &data);

protected:
    static QStringList getFieldName(Field field);

    // TorrentClient interface
public slots:
    virtual void updateStats()
    {
        //TODO
    }
};

#endif // RTORRENT_H
