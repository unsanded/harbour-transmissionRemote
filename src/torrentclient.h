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

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ clientType)

    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)


    //properties
    QString m_name;
    QString m_url;


private:
    QList<Torrent*> torrentList;
    int m_upSpeed;
    int m_downSpeed;
    QSet<QString> m_saveLocations;
    QUrl m_server;
    QString m_username, m_password;

    QMap<QString, Torrent*> torrentLookup;

protected:
    bool m_connected;

public:

    enum Field {
        ID,
        NAME,
        HASH,
        PERCENTAGE,
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

        STATUS, //status or error as a string

        STATE,

        RATIOLIMIT,

    };
    typedef int Field;

    Q_ENUMS(Field)


    //propertie getters
    int upSpeed() const;
    int downSpeed() const;
    const QStringList saveLocations() const;

Q_INVOKABLE virtual QList<Field> getAllTorrentFields() const {
        return QList<Field>();
    }

public:

    explicit TorrentClient(QString name="", QString url="", QString username="", QString password="", QObject *parent = 0)
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


void addTorrent(Torrent* torrent, QString id){
    torrentLookup.insert(id, torrent);
    torrentList.append(torrent);
}

/**
     * @brief getTorrent gets the torrent identified by an id
     * @param id the unique-per-torrent id of the torrent
     * @return  the torrent or null if no torrent has the id
     */

    Q_INVOKABLE virtual Torrent* getTorrent(QString id) const
    {
        return torrentLookup[id];
    }


QString name() const
{
    return m_name;
}

    /**
     * @brief clientType indicates what type of torrentClient this is.
     * @return  a string. for example "transmission"
     */
    virtual const char* clientType(){ return "ABSTRACT"; }


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

    void usernameChanged(QString arg);

    void passwordChanged(QString arg);

    void urlChanged(QString arg);

public slots:


    /**
     * @brief connectToServer is called when server is changed.
     * @return true when the connection succeeds.
     */
    virtual bool connectToServer(){return false;}
    virtual void disconnectFromServer(){}

    /**
     * @brief onTorrentData process data about torrents
     * @param data the data to be processed
     */
    virtual void onTorrentData(QVariantMap& /*data*/) {}


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
virtual void updateTorrents(const QVariantList &torrents = QVariantList(), const QList<int> &fields = QList<int>()){}

virtual void updateStats(){}

    /**
 * @brief uploadTorrent should upload a local torrent file to the server for downloading
 * @param torrentFile the path to the torrent file on the local system
 * @param autoStart start downloading immediately
 * @param saveLocation the location on the server to save the data
 * Note: saveLocation is not supported by rtorrent
 */
virtual void uploadTorrent(const QString& torrentFile, bool autoStart=true, QString saveLocation = "") {}

    void setUpSpeed(int arg);

    void setDownSpeed(int arg);
    void setName(QString arg)
    {
        if (m_name != arg) {
            m_name = arg;
            emit nameChanged(arg);
        }
    }

    void setUsername(QString arg)
    {
        if (m_username != arg) {
            m_username = arg;
            emit usernameChanged(arg);
        }
    }
    void setPassword(QString arg)
    {
        if (m_password != arg) {
            m_password = arg;
            emit passwordChanged(arg);
        }
    }
    void setUrl(QString arg)
    {
        disconnectFromServer();
        if (m_url != arg) {
            m_url = arg;
            emit urlChanged(arg);
        }
        connectToServer();
    }
};

#endif // TORRENTCLIENT_H
