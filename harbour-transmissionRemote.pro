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
CONFIG += C++11

QT += network

SOURCES += src/harbour-transmissionRemote.cpp \
    src/rpcCommands/torrentget.cpp \
    src/rpcconnection.cpp \
    src/torrent.cpp \
    src/transmission.cpp \
    src/settings.cpp

OTHER_FILES += qml/harbour-transmissionRemote.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    rpm/harbour-transmissionRemote.changes.in \
    rpm/harbour-transmissionRemote.spec \
    rpm/harbour-transmissionRemote.yaml \
    translations/*.ts \
    harbour-transmissionRemote.desktop \
    qml/pages/TorrentDelegate.qml \
    qml/pages/SettingsPage.qml

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-transmissionRemote-de.ts

HEADERS += \
    src/rpcCommands/torrentget.h \
    src/rpcconnection.h \
    src/torrent.h \
    src/transmission.h \
    src/settings.h

RESOURCES +=

