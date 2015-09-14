#include "updatecommand.h"

namespace rtorrentCommands {

UpdateCommand::UpdateCommand(RTorrent *parent) :
    rtorrentCommands::MultiCall(parent)
{
}

void UpdateCommand::handleReply()
{

    QVariantList arguments = replyArguments[0].toList();


    for (int torrentIndex = 0; torrentIndex< arguments.size(); torrentIndex++){
        QVariantList torrentFields = arguments.at(torrentIndex).toList();
        QVariantMap result;

        int fieldCount = torrentFields.count();
        for(int i=0; i< fieldCount; i++){
            result[fields[i]] = torrentFields[i];
        }
        emit gotTorrentInfo(result);
    }
}
} // namespace rtorrentCommands
