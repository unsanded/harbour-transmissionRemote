#ifndef RTORRENTCOMMANDS_ADDTORRENT_H
#define RTORRENTCOMMANDS_ADDTORRENT_H
#include "../xmlrpcconnection.h"

namespace rtorrentCommands {

class addTorrentCommand : public XmlRpcCommand
{
    Q_OBJECT
    Q_PROPERTY(bool autoStart READ autoStart WRITE setAutoStart NOTIFY autoStartChanged)
    bool m_autoStart;

public:
explicit addTorrentCommand(QObject *parent = 0);
bool autoStart() const;

signals:

void autoStartChanged(bool arg);

public slots:

void setAutoStart(bool arg);
};

} // namespace rtorrentCommands

#endif // RTORRENTCOMMANDS_ADDTORRENT_H
