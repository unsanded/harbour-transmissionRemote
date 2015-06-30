#ifndef TORRENTFILE_H
#define TORRENTFILE_H

#include <QObject>
#include <QVariantMap>

class Torrent;

class TorrentFile : public QObject
{
    friend class Torrent;
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(int priority READ priority WRITE setPriority NOTIFY priorityChanged)
    Q_PROPERTY(bool wanted READ wanted WRITE setWanted NOTIFY wantedChanged)
    Q_PROPERTY(long haveBytes READ haveBytes WRITE setHaveBytes NOTIFY haveBytesChanged)
    Q_PROPERTY(long completedBytes READ completedBytes WRITE setCompletedBytes NOTIFY completedBytesChanged)
    Q_PROPERTY(double percentage READ percentage NOTIFY percentageChanged)



    QString m_fileName;

    int m_priority;
    bool m_wanted;

    long m_haveBytes;

    long m_completedBytes;


protected:
    explicit TorrentFile(QObject *parent = 0);
    explicit TorrentFile(const QVariantMap &doc, QObject *parent = 0);
protected slots:
    void     updateData(const QVariantMap  &data);
public:

    QString fileName() const
{
    return m_fileName;
}

    int priority() const
{
    return m_priority;
}

    bool wanted() const
    {
        return m_wanted;
    }

    long haveBytes() const
    {
        return m_haveBytes;
    }

    long completedBytes() const
    {
        return m_completedBytes;
    }

    double percentage() const
    {
        if(completedBytes() && haveBytes())
            return 100.0*((double)haveBytes())/((double)completedBytes());
        return -1;

    }

signals:

    void fileNameChanged(QString arg);

    void priorityChanged(int arg);

    void wantedChanged(bool arg);

    void haveBytesChanged(long arg);

    void completedBytesChanged(long arg);

    void percentageChanged(double arg);

public slots:

    void setFileName(QString arg)
{
    if (m_fileName != arg) {
        m_fileName = arg;
        emit fileNameChanged(arg);
    }
}
    void setPriority(int arg)
{
    if (m_priority != arg) {
        m_priority = arg;
        emit priorityChanged(arg);
    }
    }
    void setWanted(bool arg)
    {
        if (m_wanted != arg) {
            m_wanted = arg;
            emit wantedChanged(arg);
        }
    }
    void setHaveBytes(long arg)
    {
        if (m_haveBytes != arg) {
            m_haveBytes = arg;
            emit haveBytesChanged(arg);
            emit percentageChanged(percentage());
        }
    }
    void setCompletedBytes(long arg)
    {
        if (m_completedBytes != arg) {
            m_completedBytes = arg;
            emit completedBytesChanged(arg);
            emit percentageChanged(percentage());
        }
    }
};

#endif // TORRENTFILE_H
