#include "torrentfile.h"

#include <QJsonObject>

TorrentFile::TorrentFile(QObject *parent) :
    QObject(parent)
{

}

TorrentFile::TorrentFile(const QVariantMap &doc, QObject *parent):
    QObject(parent)
{
    updateData(doc);
}

void TorrentFile::updateData(const QVariantMap &doc)
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


             //from here only property setters//
/////////////////////////////////////////////////////////////////////



void TorrentFile::setFileName(QString arg)
{
    if (m_fileName != arg) {
        m_fileName = arg;
        emit fileNameChanged(arg);
    }
}

void TorrentFile::setPriority(int arg)
{
    if (m_priority != arg) {
        m_priority = arg;
        emit priorityChanged(arg);
    }
}

void TorrentFile::setWanted(bool arg)
{
    if (m_wanted != arg) {
        m_wanted = arg;
        emit wantedChanged(arg);
    }
}

void TorrentFile::setHaveBytes(long arg)
{
    if (m_haveBytes != arg) {
        m_haveBytes = arg;
        emit haveBytesChanged(arg);
        emit percentageChanged(percentage());
    }
}

void TorrentFile::setCompletedBytes(long arg)
{
    if (m_completedBytes != arg) {
        m_completedBytes = arg;
        emit completedBytesChanged(arg);
        emit percentageChanged(percentage());
    }
}
