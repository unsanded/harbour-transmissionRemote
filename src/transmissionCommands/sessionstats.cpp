#include "sessionstats.h"

namespace transmissionCommands {

SessionStats::SessionStats(QObject *parent) :
    JsonRpcCommand("session-stats", parent)
{

}

void SessionStats::handleReply()
{
    emit gotDownSpeed(replyArguments["downloadSpeed"].toInt());
    emit gotUpspeed(replyArguments["uploadSpeed"].toInt());
}

} // namespace transmissionCommands

