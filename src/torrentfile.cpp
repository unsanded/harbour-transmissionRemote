#include "torrentfile.h"

#include <QJsonObject>

TorrentFile::TorrentFile(QObject *parent) :
    QObject(parent)
{

}

TorrentFile::TorrentFile(QJsonObject &doc, QObject *parent):
    QObject(parent)
{
    parseJson(doc);
}

void TorrentFile::parseJson(QJsonObject &doc)
{
    if(doc.contains("name"))
        setFileName(doc["name"].toString());
    if(doc.contains("priority"))
        setPriority(doc["priority"].toInt());
    if(doc.contains("wanted"))
        setWanted(  doc["wanted"].toBool());
    if(doc.contains("bytesCompleted"))
        setHaveBytes(doc["bytesCompleted"].toInt());
    // this might give trouble with long files
    // if that happens, it should be changed to toDouble()

    if(doc.contains("length"))
        setCompletedBytes(doc["length"].toInt());

}
