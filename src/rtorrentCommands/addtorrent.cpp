#include "addtorrent.h"

#include <QFile>

namespace rtorrentCommands {


addTorrentCommand::addTorrentCommand(QString filename, QString downloadDir, QObject *parent) :
    XmlRpcCommand("load_raw_start", parent)
{
    m_autoStart=true;
    QFile file(filename);
    if(!file.exists())
    {
        qWarning() << "uploading nonexistant torrent file " << filename;
        return;
    }
    file.open(QFile::ReadOnly);

    if(!file.isOpen()){
        qWarning() << "unable to open torrent file " << filename;
    }

    QByteArray data = file.readAll();
    requestArguments.append(data);

}

bool addTorrentCommand::autoStart() const
{
    return m_autoStart;
}

void addTorrentCommand::setAutoStart(bool arg)
{
    if (m_autoStart != arg) {
        m_autoStart = arg;
        method = m_autoStart? "load_raw_start": "load_raw";
        emit autoStartChanged(arg);
    }
}

} // namespace rtorrentCommands
