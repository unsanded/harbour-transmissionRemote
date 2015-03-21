# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-transmissionRemote

CONFIG += sailfishapp

QT += network

SOURCES += src/harbour-transmissionRemote.cpp \
    rpcconnection.cpp \
    torrentmodel.cpp \
    torrent.cpp \
    transmission.cpp

OTHER_FILES += qml/harbour-transmissionRemote.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    rpm/harbour-transmissionRemote.changes.in \
    rpm/harbour-transmissionRemote.spec \
    rpm/harbour-transmissionRemote.yaml \
    translations/*.ts \
    harbour-transmissionRemote.desktop \
    qml/pages/TorrentDelegate.qml

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-transmissionRemote-de.ts

HEADERS += \
    rpcconnection.h \
    torrentmodel.h \
    torrent.h \
    transmission.h

RESOURCES +=

