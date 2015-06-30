#include "sessionstats.h"

namespace transmissionCommands {

SessionStats::SessionStats(QObject *parent) :
    JsonRpcCommand("session-stats", parent)
{

}

void SessionStats::handleReply()
{
    emit gotDownSpeed(reply.arguments["downloadSpeed"].toInt());
    emit gotUpspeed(reply.arguments["uploadSpeed"].toInt());
}

} // namespace transmissionCommands

