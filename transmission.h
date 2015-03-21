#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include "torrent.h"

#include <QObject>
#include <QQmlListProperty>

class Transmission : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Torrent*> torrents READ torrents  NOTIFY torrentsChanged)
    QQmlListProperty<Torrent*> m_torrents;
    QList<Torrent*> torrentList;



public:
    explicit Transmission(QObject *parent = 0);

QQmlListProperty<Torrent*> torrents() const
{
    return m_torrents;
}

signals:

void torrentsChanged(QQmlListProperty<Torrent*> arg);

public slots:

};

#endif // TRANSMISSION_H
