#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QQmlListProperty>
#include "torrentclient.h"


class Settings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<TorrentClient> clients READ clients NOTIFY clientsChanged)

    QSettings settings;

    QList<TorrentClient*> clientList;

protected:
    QMap<QString, TorrentClient *> m_clients;

public:
    explicit Settings(QObject *parent = 0);

    ~Settings(){
        saveClients();
    }

QQmlListProperty<TorrentClient> clients()
{
    clientList = m_clients.values();
    return QQmlListProperty<TorrentClient>((QObject*) this, clientList);
}

signals:


void clientsChanged(QQmlListProperty<TorrentClient> arg);

public:
Q_INVOKABLE TorrentClient* addClient(QString type, QString name, QString url, QString username, QString password);
Q_INVOKABLE TorrentClient* getClient(QString name);
Q_INVOKABLE void removeClient(QString name);

public slots:

void saveClients();
void loadClients();



};

#endif // SETTINGS_H
