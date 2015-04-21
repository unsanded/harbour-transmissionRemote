#include "torrent.h"
#include "rpcCommands/torrentget.h"
#include "torrentfile.h"
#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>
Torrent::Torrent(RpcConnection* connection, QObject *parent) :
    QObject(parent),
    connection(connection)
{
    setfileCount(1);
}

Torrent::Torrent(const Torrent &other):
    QObject(parent())
{
    m_fileCount=other.m_fileCount;
    connection=other.connection;
    setParent(other.parent());
    setid(other.id());
    setname(other.name());
}


void Torrent::updateFields(QJsonObject &freshData){

    qDebug() << "updating fields";

    if(freshData.contains("name"))
        setname(freshData["name"].toString());
    if(freshData.contains("percentDone"))
        setpercentage(freshData["percentDone"].toDouble() * 100.0);

    if(freshData.contains("files")){
        for(auto iter : freshData["files"].toArray()){
            QJsonObject fileObject=iter.toObject();

            QString filename  = fileObject["name"].toString() ;
            TorrentFile* file = fileLookup[filename];
            if(!file){
                    file=new TorrentFile(this);
                    fileLookup.insert(filename, file);
                    fileList.append(file);
            }
            file->parseJson(fileObject);
        }
        if(freshData.contains("fileStats")){
            int i=0;
            for(auto iter : freshData["fileStats"].toArray()){

                if(i>=fileList.size()) break;

                auto obj=iter.toObject();
                fileList[i++]->parseJson(obj);
            }
        }
        setfileCount(fileList.size());
        emit filesChanged(files());
    }
}

void Torrent::fullUpdate()
{
    QList<int> list;
    list << id();
    RpcCommands::TorrentGet* command= new RpcCommands::TorrentGet(list, QStringList(torrentFields), this);
    connect(
                command, SIGNAL(gotTorrentInfo(QJsonObject&)),
                this     , SLOT(updateFields  (QJsonObject&))
            );

    connection->sendCommand(command);
}
