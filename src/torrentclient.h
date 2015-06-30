#ifndef TORRENTCLIENT_H
#define TORRENTCLIENT_H

#include <QObject>
#include <QQmlListProperty>
#include <QFile>
#include <QSet>
#include <QUrl>
#include <QStringList>

class Torrent;

/**
 * @brief The torrentClient class describes a torrent client. It is virtual, so should be subclassed.
 */

class TorrentClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Torrent> torrents READ torrents  NOTIFY torrentsChanged)
    Q_PROPERTY(QUrl server READ server WRITE setServer NOTIFY serverChanged)
    Q_PROPERTY(int upSpeed READ upSpeed WRITE setUpSpeed NOTIFY upSpeedChanged)
    Q_PROPERTY(int downSpeed READ downSpeed WRITE setDownSpeed NOTIFY downSpeedChanged)
    Q_PROPERTY(QStringList saveLocations READ saveLocations NOTIFY saveLocationsChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

    //properties
protected:
    QList<Torrent*> torrentList;
    int m_upSpeed;
    int m_downSpeed;
    QSet<QString> m_saveLocations;

    //propertie getters
    int upSpeed() const;
    int downSpeed() const;
    const QStringList saveLocations() const;

    QUrl m_server;

    bool m_connected;

public:
    explicit TorrentClient(QObject *parent = 0)
        :QObject(parent)
    {
        m_upSpeed=0;
        m_downSpeed=0;
    }

    /**
 * @brief torrents
 * @return  all the torrents as a QQmlListProperty, so that they are accessable in qml.
 */

QQmlListProperty<Torrent> torrents();

/**
 * @brief connected
 * @return  whether we are connected
 */
bool connected() const;


/**
     * @brief getTorrent gets the torrent identified by an id
     * @param id the unique-per-torrent id of the torrent
     * @return  the torrent or null if no torrent has the id
     */
    virtual Torrent* getTorrent(const QVariant id) const =0;


/**
 * @brief server The server this client is connected to
 * @return  the server
 */
QUrl server() const;

signals:

    void torrentsChanged(QQmlListProperty<Torrent> arg);
    void serverChanged(QUrl arg);
    void upSpeedChanged(int arg);
    void downSpeedChanged(int arg);
    void saveLocationsChanged(QStringList arg);
    void connectedChanged(bool arg);

public slots:
    /**
     * @brief connectToServer is called when server is changed.
     * @return true when the connection succeeds.
     */
    virtual bool connectToServer()=0;


    /**
 * @brief addSaveLocation adds a suggested save-location
 * @param location the location to be added
 * @return whether the save-location was already inserted
 */
virtual bool addSaveLocation(QString location);

/**
 * @brief updateTorrents send an update request to the the server
 * @param torrents the id's of the torrents to update
 * @param fields the fields to upate
 */
virtual void updateTorrents(const QVariantList& torrents = QVariantList(), const QStringList &fields = QStringList())=0;



    void setServer(QUrl arg);

    void setUpSpeed(int arg);

    void setDownSpeed(int arg);
};



#endif // TORRENTCLIENT_H
