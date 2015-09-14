#ifndef RTORRENTTORRENT_H
#define RTORRENTTORRENT_H
#include "rtorrent.h"
#include "torrent.h"

class RTorrentTorrent : public Torrent
{
    Q_OBJECT
public:
    explicit RTorrentTorrent(RTorrent *parent );

    virtual void updateFields(QVariantMap &freshData);
    virtual void moveData(QString /*destination*/){
        //not supported for now
    };
signals:

public slots:

};

#endif // RTORRENTTORRENT_H
