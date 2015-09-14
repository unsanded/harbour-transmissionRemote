#include "transmissiontorrent.h"
#include "transmissionCommands/movetorrent.h"


TransmissionTorrent::TransmissionTorrent(Transmission *parent) :
    Torrent(parent)
{
}

void TransmissionTorrent::moveData(QString destination)
{
    transmissionCommands::moveTorrent* command = new transmissionCommands::moveTorrent(this, destination);
    ((Transmission*)parent())->connection->sendCommand(command);

    connect(
                command, SIGNAL(done()),
                command, SLOT(deleteLater())
            );
}



void TransmissionTorrent::updateFields(QVariantMap &freshData){

    if(freshData.contains("name"))
        setname(freshData["name"].toString());
    if(freshData.contains("percentDone"))
        setpercentage(freshData["percentDone"].toDouble() * 100.0);

    if(freshData.contains("downSpeed")){
        setUpSpeed(freshData["upSpeed"].toInt());
    }
    if(freshData.contains("downSpeed")){
        setDownSpeed(freshData["upSpeed"].toInt());
    }
    if(freshData.contains("downloadDir")){
        setDownloadDir(freshData["downloadDir"].toString());
    }
    if(freshData.contains("files")){
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
