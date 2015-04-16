#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString transmissionHost READ transmissionHost WRITE setTransmissionHost NOTIFY transmissionHostChanged)
    Q_PROPERTY(short unsigned port READ port WRITE setPort NOTIFY portChanged)

    QSettings settings;


public:
    explicit Settings(QObject *parent = 0);

QString transmissionHost() const
{
    return settings.value("transmissionHost").toString();
}

short unsigned port() const
{
    return settings.value("transmissionHost").toInt();
}

signals:

void transmissionHostChanged(QString arg);

void portChanged(short unsigned arg);

public slots:

void setTransmissionHost(QString arg)
{
    if (transmissionHost() != arg) {
        settings.setValue("transmissionHost", arg);
        emit transmissionHostChanged(arg);
    }
}
void setPort(short unsigned arg)
{
    if (port() != arg) {
        settings.setValue("port", arg);
        emit portChanged(arg);
    }
}
};

#endif // SETTINGS_H
