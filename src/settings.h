#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QQmlListProperty>
#include "torrentclient.h"


class Settings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool autoUpdateInClientSelect READ autoUpdateInClientSelect WRITE setAutoUpdateInClientSelect NOTIFY autoUpdateInClientSelectChanged)
    Q_PROPERTY(QQmlListProperty<TorrentClient> clients READ clients NOTIFY clientsChanged)
    QSettings settings;
    QList<TorrentClient*> clientList;

    bool m_autoUpdateInClientSelect;

protected:
    QMap<QString, TorrentClient *> m_clients;

public:
    explicit Settings(QObject *parent = 0);

    ~Settings(){
        saveSettings();
    }

QQmlListProperty<TorrentClient> clients()
{
    clientList = m_clients.values();
    return QQmlListProperty<TorrentClient>((QObject*) this, clientList);
}

signals:


void clientsChanged(QQmlListProperty<TorrentClient> arg);

void autoUpdateInClientSelectChanged(bool arg);

public:
Q_INVOKABLE TorrentClient* addClient(QString type, QString name, QString url, QString username, QString password);
Q_INVOKABLE TorrentClient* getClient(QString name);
Q_INVOKABLE void removeClient(QString name);

bool autoUpdateInClientSelect() const
{
    return m_autoUpdateInClientSelect;
}

public slots:

void saveClients();
void loadClients();
void saveSettings();



void setAutoUpdateInClientSelect(bool arg)
{
    if (m_autoUpdateInClientSelect != arg) {
        m_autoUpdateInClientSelect = arg;
        emit autoUpdateInClientSelectChanged(arg);
    }
}
};

#endif // SETTINGS_H
