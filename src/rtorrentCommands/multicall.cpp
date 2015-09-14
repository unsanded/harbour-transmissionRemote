#include "multicall.h"

namespace rtorrentCommands{


MultiCall::MultiCall(RTorrent *parent) :
    XmlRpcCommand("d.multicall", parent)
{

    requestArguments.append("");

}












}//namespace
