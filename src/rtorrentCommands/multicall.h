#include <src/rtorrent.h>
#include <src/xmlrpcconnection.h>

#ifndef MULTICALL_H
#define MULTICALL_H

namespace rtorrentCommands {

class MultiCall : public XmlRpcCommand
{
    Q_OBJECT
    QStringList methods;
public:
    explicit MultiCall(RTorrent *parent = 0);

    void addMethod(QString method){
          methods << method;
          requestArguments.append(method + "=");
    }


signals:

public slots:

};

}//namespace
#endif // MULTICALL_H
