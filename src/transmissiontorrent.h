#include "torrent.h"
#include "transmission.h"

#ifndef TRANSMISSIONTORRENT_H
#define TRANSMISSIONTORRENT_H


class TransmissionTorrent : public Torrent
{
    Q_OBJECT
public:
    explicit TransmissionTorrent(Transmission *parent = 0);

    virtual void moveData(QString destination);
    virtual void updateFields(QVariantMap& freshData);



signals:

public slots:

};

#endif // TRANSMISSIONTORRENT_H
