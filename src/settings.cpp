#include "settings.h"


#include "clienttypes.h"
#include "qdebug.h"

Settings::Settings(QObject *parent) :
    QObject(parent)
{
    //migrate from old vesion
    if(settings.contains("transmissionHost")){
        QString host = settings.value("transmissionHost").toString();
        int port = settings.value("transmissionPort", 9091).toInt();

        settings.remove("transmissionHost");
        settings.remove("transmissionPort");

        QString url = QString("http://%1:%2/transmission/rpc").arg(host).arg(port);
        TorrentClient* tm = ClientTypes::makeTorrentClient("transmission", "Transmission", url, "", "");
        m_clients.insert("Transmission", tm);
    }
    else
    {
      loadClients();
    }
}

void Settings::saveClients()
{
    settings.beginWriteArray("clients", m_clients.size());
    int i=0;
    for(TorrentClient* client : m_clients){
       settings.setArrayIndex(i++);
       settings.setValue("name", client->name());
       settings.setValue("type", client->clientType());
       settings.setValue("url", client->url());
       settings.setValue("password", client->password());
       settings.setValue("username", client->username());
    }
    settings.endArray();
    settings.sync();
}

void Settings::loadClients()
{


    for(TorrentClient* client : m_clients){
        delete client;
    }
    m_clients.clear();

    int i=0;
    int size = settings.beginReadArray("clients");
    while(i<size){

       settings.setArrayIndex(i++);

       QString name, type, url, password, username;

       type = settings.value("type").toString();

       name = settings.value("name", type + QString::number(i)) .toString();
       url  = settings.value("url").toString();
       password=settings.value("password", "").toString();
       username=settings.value("username", "").toString();
       addClient(type, name, url, username, password);
    }
    settings.endArray();

}

void Settings::saveSettings()
{
    settings.beginGroup("gui");
    settings.setValue("autoUpdateInClientSelect", autoUpdateInClientSelect());
    settings.endGroup();
    saveClients();
}

TorrentClient* Settings::getClient(QString name)
{
    if(m_clients.contains(name))
        return m_clients[name];
    return nullptr;
}

void Settings::removeClient(QString name)
{
    if(!m_clients.contains(name))
    {
        qWarning() << "trying to remove non-existant client " << name;
    }
    TorrentClient* client = m_clients[name];
    clientList.removeAll(client);
    m_clients.remove(name);
    client->deleteLater();

}

TorrentClient*  Settings::addClient(QString type, QString name, QString url, QString username, QString password) {
    TorrentClient* client = ClientTypes::makeTorrentClient(type, name, url, username, password);

    if(client == nullptr)
       return nullptr;
    m_clients.insert(name, client);
    emit clientsChanged(clients());
    return client;
}





