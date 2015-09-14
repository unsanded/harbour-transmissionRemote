#include "rtorrenttorrent.h"

RTorrentTorrent::RTorrentTorrent(RTorrent *parent) :
    Torrent(parent)
{
}

void RTorrentTorrent::updateFields(QVariantMap &freshData)
{
    if(freshData.contains("name"))
        setname(freshData["name"].toString());

    if(freshData.contains("completed_bytes")){
        setpercentage(freshData["compleded_bytes"].toDouble()/((double) totalSize()) * 100.0);
    }

    if(freshData.contains("downSpeed")){
        setUpSpeed(freshData["upSpeed"].toInt());
    }
    if(freshData.contains("downSpeed")){
        setDownSpeed(freshData["upSpeed"].toInt());
    }
    if(freshData.contains("")){
        for(auto iter : freshData["files"].toList()){
            QVariantMap fileObject=iter.toMap();

            QString filename  = fileObject["name"].toString() ;
            TorrentFile* file = fileLookup[filename];
            if(!file){
                file = addFile(filename);
            }
            file->updateData(fileObject);
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

