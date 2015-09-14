#include "addtorrent.h"

namespace rtorrentCommands {


addTorrentCommand::addTorrentCommand(QObject *parent) :
    XmlRpcCommand("load_raw_start", parent)
{
    m_autoStart=true;
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
