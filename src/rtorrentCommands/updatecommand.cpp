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
            if(fields[i] == "hash")
                result["hash"] = result["id"] = torrentFields[i].toString();
            else
            if(fields[i] == "name")
                result["name"] = torrentFields[i];
            else
            if(fields[i] == "state")
                result["state"] = torrentFields[i];
            else
            if(fields[i] == "up_rate")
                result["upSpeed"] = torrentFields[i];
            else
            if(fields[i] == "down_rate")
                result["downSpeed"] = torrentFields[i];
            else
            if(fields[i] == "message")
                result["status"] = torrentFields[i];
        }
        emit gotTorrentInfo(result);
    }
}
} // namespace rtorrentCommands
