#ifndef TORRENT_H
#define TORRENT_H

#include <QObject>
#include <QVariantMap>

class Torrent : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setid NOTIFY idChanged)

    Q_PROPERTY(QString name READ name WRITE setname NOTIFY nameChanged)
    Q_PROPERTY(qreal percentage READ percentage WRITE setpercentage NOTIFY percentageChanged)
    Q_PROPERTY(int fileCount READ fileCount WRITE setfileCount NOTIFY fileCountChanged)
    //TODO: figure out best way to represent state

    QString m_name;
    qreal m_percentage;
    int m_fileCount;

    int m_id;

    QVariantMap fields;

public:
    explicit Torrent(QObject *parent = 0);
    explicit Torrent(const Torrent& other );


QString name() const
{
    return m_name;
}

qreal percentage() const
{
    return m_percentage;
}

int fileCount() const
{
    return m_fileCount;
}

int id() const
{
    return m_id;
}

signals:

void nameChanged(QString arg);

void percentageChanged(qreal arg);

void fileCountChanged(int arg);

void idChanged(int arg);

public slots:

void updateFields(QJsonObject& freshData);


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
void setid(int arg)
{
    if (m_id != arg) {
        m_id = arg;
        emit idChanged(arg);
    }
}
};

#endif // TORRENT_H
