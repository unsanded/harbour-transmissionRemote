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
    Q_PROPERTY(int upSpeed READ upSpeed WRITE setUpSpeed NOTIFY upSpeedChanged)
    Q_PROPERTY(int downSpeed READ downSpeed WRITE setDownSpeed NOTIFY downSpeedChanged)
    Q_PROPERTY(QStringList saveLocations READ saveLocations NOTIFY saveLocationsChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString type READ clientType)

    Q_PROPERTY(QString username READ username)
    Q_PROPERTY(QString password READ password)
    Q_PROPERTY(QString url READ url)

    //properties
    QString m_name;

    QString m_url;

protected:
    QList<Torrent*> torrentList;
    int m_upSpeed;
    int m_downSpeed;
    QSet<QString> m_saveLocations;
    QUrl m_server;
    bool m_connected;

    QString m_username, m_password;

public:

    typedef  enum Field {
        ID,
        NAME,
        DOWNLOADDIR,
        FILES,
        FILECOUNT,
        DOWNLOADED,
        UPLOADED,
        SIZE,
        PEERS,
        PEERSGIVING,
        PEERSGETTING,
        SEEDS,
        ETA,
        DOWNSPEED,
        UPSPEED,

        ERROR,

        STATE,

        RATIOLIMIT,

    } Field;

    Q_ENUMS(Field)


    //propertie getters
    int upSpeed() const;
    int downSpeed() const;
    const QStringList saveLocations() const;

Q_INVOKABLE virtual QStringList getAllTorrentFields() const = 0;

public:

    explicit TorrentClient(QString name, QString url, QString username="", QString password="", QObject *parent = 0)
        :QObject(parent)
    {

        m_upSpeed = 0;
        m_downSpeed = 0;
        m_name = name;
        m_username = username;
        m_password = password;
        m_url = url;
    }

    bool operator<(const TorrentClient& other){
        return m_name < other.m_name;
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
    Q_INVOKABLE virtual Torrent* getTorrent(int id) const =0;



QString name() const
{
    return m_name;
}

    /**
     * @brief clientType indicates what type of torrentClient this is.
     * @return  a string. for example "transmission"
     */
    virtual const char* clientType()=0;

QString password() const
{
    return m_password;
}

QString username() const
{
    return m_username;
}

QString url() const
{
    return m_url;
}

signals:

    void torrentsChanged(QQmlListProperty<Torrent> arg);
    void serverChanged(QUrl arg);
    void upSpeedChanged(int arg);
    void downSpeedChanged(int arg);
    void saveLocationsChanged(QStringList arg);
    void connectedChanged(bool arg);

    void nameChanged(QString arg);

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
virtual void updateTorrents(const QVariantList& torrents = QVariantList(), const QList<Field> &fields = QList<Field>())=0;


    void setUpSpeed(int arg);

    void setDownSpeed(int arg);
    void setName(QString arg)
    {
        if (m_name != arg) {
            m_name = arg;
            emit nameChanged(arg);
        }
    }
};



#endif // TORRENTCLIENT_H
