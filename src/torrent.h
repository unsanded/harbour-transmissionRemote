#ifndef TORRENT_H
#define TORRENT_H

#include <QObject>
#include <QVariantMap>
#include <QQmlListProperty>
#include "torrentclient.h"
#include "torrentfile.h"
#include "torrentclient.h"


class Torrent : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)

    Q_PROPERTY(QString id READ id WRITE setid NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setname NOTIFY nameChanged)
    Q_PROPERTY(qreal percentage READ percentage WRITE setpercentage NOTIFY percentageChanged)
    Q_PROPERTY(int fileCount READ fileCount WRITE setfileCount NOTIFY fileCountChanged)
    Q_PROPERTY(int upSpeed READ upSpeed WRITE setUpSpeed NOTIFY upSpeedChanged)
    Q_PROPERTY(int downSpeed READ downSpeed WRITE setDownSpeed NOTIFY downSpeedChanged)
    Q_PROPERTY(int totalSize READ totalSize WRITE setTotalSize NOTIFY totalSizeChanged)
    Q_PROPERTY(QString downloadDir READ downloadDir WRITE setDownloadDir NOTIFY downloadDirChanged)
    Q_PROPERTY(bool isActive READ isActive WRITE setIsActive NOTIFY isActiveChanged)
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)

    Q_PROPERTY(QQmlListProperty<TorrentFile> files READ files NOTIFY filesChanged)

    QVariantMap extraData;


public:
    typedef enum State{
        DOWNLOADING,
        SEEDING,
        IDLE,
        CHECKING,
        MOVING,
        STALLED
    } State;


private:
    QString m_name;
    qreal m_percentage;
    int m_fileCount;

    QString m_id;

    QVariantMap fields;

protected:
    QList<TorrentFile*> fileList;
    QMap<QString, TorrentFile*> fileLookup;

private:
    int m_upSpeed;
    int m_downSpeed;
    int m_totalSize;
    QString m_downloadDir;
    bool m_isActive;
    State m_state;

public:
    explicit Torrent( TorrentClient *parent=0);
    explicit Torrent( const Torrent& other );

protected:
    /**
     * for internal use only. Adds a file to the list of files
     */
    TorrentFile* addFile(QString filename);

public:

    //properties
QString name() const;
qreal percentage() const;
int fileCount() const;
QString id() const;
int upSpeed() const;
int downSpeed() const;
int totalSize() const;
QQmlListProperty<TorrentFile> files();
QString downloadDir() const;
bool isActive() const;
State state() const;

public slots:

/**
 * @brief updateFields updates the torrent data with the data in freshData
 * @param freshData VariantMap which maps field names to torrentData.
 * The fieldnames depend on which client is used. So to add a new client one needs so subclass torrent as wel
 */
virtual void updateFields(QVariantMap& /*freshData*/){}

/**
 * @brief moveData this function should move the data on the server for this torrent to a given location
 */
virtual void moveData(QString /*destination*/){}

/**
 * @brief fullUpdate should update all field and files for this torrent
 */
void fullUpdate();

signals:

/**
 * @brief finished should be emitted when the torrent is completed
 */
void finished();

//property signals...
void nameChanged(QString arg);
void percentageChanged(qreal arg);
void fileCountChanged(int arg);
void idChanged(QString arg);
void filesChanged(QQmlListProperty<TorrentFile> arg);
void upSpeedChanged(int arg);
void downSpeedChanged(int arg);
void totalSizeChanged(int arg);
void downloadDirChanged(QString arg);
void isActiveChanged(bool arg);
void stateChanged(State arg);







public slots:


// From now on it's just setters; boooring
void setname(QString arg)
{
    if (m_name != arg) {
        m_name = arg;
        emit nameChanged(arg);
    }
}
void setpercentage(qreal arg)
{
    if (m_percentage != arg) {
        m_percentage = arg;
        emit percentageChanged(arg);
    }
}
void setfileCount(int arg)
{
    if (m_fileCount != arg) {
        m_fileCount = arg;
        emit fileCountChanged(arg);
    }
}
void setid(QString arg)
{
    if (m_id != arg) {
        m_id = arg;
        emit idChanged(arg);
    }
}
void setUpSpeed(int arg)
{
    if (m_upSpeed != arg) {
        m_upSpeed = arg;
        emit upSpeedChanged(arg);
    }
}
void setDownSpeed(int arg)
{
    if (m_downSpeed != arg) {
        m_downSpeed = arg;
        emit downSpeedChanged(arg);
    }
}
void setTotalSize(int arg)
{
    if (m_totalSize != arg) {
        m_totalSize = arg;
        emit totalSizeChanged(arg);
    }
}
void setDownloadDir(QString arg)
{
    if (m_downloadDir != arg) {
        m_downloadDir = arg;
        emit downloadDirChanged(arg);
    }
}
void setIsActive(bool arg)
{
    if (m_isActive != arg) {
        m_isActive = arg;
        emit isActiveChanged(arg);
    }
}
void setState(State arg)
{
    if (m_state != arg) {
        m_state = arg;
        emit stateChanged(arg);
    }
}
};



#endif // TORRENT_H
