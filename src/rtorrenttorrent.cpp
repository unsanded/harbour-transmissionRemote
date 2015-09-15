#include "rtorrenttorrent.h"

RTorrentTorrent::RTorrentTorrent(RTorrent *parent) :
    Torrent(parent)
{
}

void RTorrentTorrent::updateFields(QVariantMap &freshData)
{
    if(freshData.contains("name"))
        setname(freshData["name"].toString());

    if(freshData.contains("size_bytes"))
        setTotalSize(freshData["size_bytes"].toInt());

    if(freshData.contains("completed_bytes")){
        setpercentage(freshData["compleded_bytes"].toDouble()/((double) totalSize()) * 100.0);
    }

    if(freshData.contains("up_rate")){
        setUpSpeed(freshData["up_rate"].toInt());
    }
    if(freshData.contains("down_rate")){
        setDownSpeed(freshData["down_rate"].toInt());
    }

}

