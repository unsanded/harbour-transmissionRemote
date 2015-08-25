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
    virtual QStringList getAllTorrentFields() const
    {
        return QStringList();
    }

    virtual Torrent *getTorrent(QString id) const
    {
        return torrentLookup[id];
    }
    virtual const char *clientType()
    {
        return "rtorrent";
    }


public slots:
    virtual bool connectToServer()
    {
    }
    virtual void disconnectFromServer();

    virtual void updateTorrents(const QVariantList& torrents = QVariantList(), const QList<Field> &fields = QList<Field>());

    void onTorrentData(QVariantMap &data);

protected:
    static QStringList getFieldName(Field field);
};

#endif // RTORRENT_H
