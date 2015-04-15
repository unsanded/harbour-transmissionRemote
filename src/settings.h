#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString transmissionHost READ transmissionHost WRITE setTransmissionHost NOTIFY transmissionHostChanged)

    QSettings settings;

public:
    explicit Settings(QObject *parent = 0);

QString transmissionHost() const
{
    return settings.value("transmissionHost").toString();
}

signals:

void transmissionHostChanged(QString arg);

public slots:

void setTransmissionHost(QString arg)
{
    if (transmissionHost() != arg) {
        settings.setValue("transmissionHost", arg);
        emit transmissionHostChanged(arg);
    }
}
};

#endif // SETTINGS_H
