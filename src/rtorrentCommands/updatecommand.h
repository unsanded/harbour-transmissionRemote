#ifndef RTORRENTCOMMANDS_UPDATECOMMAND_H
#define RTORRENTCOMMANDS_UPDATECOMMAND_H
#include "multicall.h"

#include <src/rtorrent.h>
namespace rtorrentCommands {

class UpdateCommand : public rtorrentCommands::MultiCall
{
    Q_OBJECT
public:
    explicit UpdateCommand(RTorrent *parent = 0);

    inline void addField(QStringList fields){
        for (auto field:fields){
            addField(field);
        }
    }

    inline void addField(QString field){
        MultiCall::addMethod(QString("d.get_%1").arg(field));

    }

signals:

public slots:

};

} // namespace rtorrentCommands

#endif // RTORRENTCOMMANDS_UPDATECOMMAND_H
