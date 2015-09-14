#include "torrent.h"
#include "torrentfile.h"
#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>
Torrent::Torrent(TorrentClient *parent) :
    QObject(parent)
{
    setfileCount(0);
}

Torrent::Torrent(const Torrent &other):
    QObject((TorrentClient*) other.parent())
{
    m_fileCount=other.m_fileCount;
    setParent(other.parent());
    setid(other.id());
    setname(other.name());
}


void Torrent::updateFields(QVariantMap &freshData){

    if(freshData.contains("name"))
        setname(freshData["name"].toString());
    if(freshData.contains("percentDone"))
        setpercentage(freshData["percentDone"].toDouble() * 100.0);

    if(freshData.contains("files")){
        for(auto iter : freshData["files"].toList()){
            QVariantMap fileObject=iter.toMap();

            QString filename  = fileObject["name"].toString() ;
            TorrentFile* file = fileLookup[filename];
            if(!file){
                    file=new TorrentFile(this);
                    fileLookup.insert(filename, file);
                    fileList.append(file);
            }
            file->updateData(fileObject);
        }
        if(freshData.contains("downSpeed")){
            setUpSpeed(freshData["upSpeed"].toInt());
        }
        if(freshData.contains("downSpeed")){
            setDownSpeed(freshData["upSpeed"].toInt());
        }
        if(freshData.contains("fileStats")){
            int i=0;
            for(auto iter : freshData["fileStats"].toList()){

                if(i>=fileList.size()) break;

                auto obj=iter.toMap();
                fileList[i++]->updateData(obj);
            }
        }
        setfileCount(fileList.size());
        emit filesChanged(files());
    }
}

void Torrent::fullUpdate()
{
    QVariantList list;
    list << id();
    QList<TorrentClient::Field> fields;// = ((TorrentClient*) parent())->getAllTorrentFields();
    fields.append( TorrentClient::FILES);



    ((TorrentClient*) parent())->updateTorrents(list, fields);

}
