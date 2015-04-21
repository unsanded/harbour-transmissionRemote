#include "sessionstats.h"

namespace RpcCommands {

SessionStats::SessionStats(QObject *parent) :
    RpcCommand("session-stats", parent)
{

}

} // namespace RpcCommands


void RpcCommands::SessionStats::handleReply()
{
    emit gotDownSpeed(reply.arguments["downloadSpeed"].toInt());
    emit gotUpspeed(reply.arguments["uploadSpeed"].toInt());
}
