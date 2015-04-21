#include <src/rpcconnection.h>

#ifndef RPCCOMMANDS_SESSIONSTATS_H
#define RPCCOMMANDS_SESSIONSTATS_H

namespace RpcCommands {

class SessionStats : public RpcCommand
{
    Q_OBJECT
public:
    explicit SessionStats(QObject *parent = 0);


signals:
    void gotUpspeed(int upspeed);
    void gotDownSpeed(int downSpeed);


public slots:


    // RpcCommand interface
public slots:
    virtual void handleReply();
};

} // namespace RpcCommands

#endif // RPCCOMMANDS_SESSIONSTATS_H
