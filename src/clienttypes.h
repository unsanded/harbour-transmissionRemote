#ifndef CLIENTTYPES_T_H
#define CLIENTTYPES_T_H

#include "torrentclient.h"



namespace ClientTypes {

TorrentClient* makeTorrentClient(QString type, QString name, QString url, QString username, QString password, QVariantMap extraSettings=QVariantMap());


} // namespace ClientTypes

#endif // CLIENTTYPES_T_H
