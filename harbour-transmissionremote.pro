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
TARGET = harbour-transmissionremote

CONFIG += sailfishapp
CONFIG += C++11

QT += network

SOURCES += \
    src/rpcconnection.cpp \
    src/torrent.cpp \
    src/transmission.cpp \
    src/settings.cpp \
    src/torrentfile.cpp \
    src/harbour-transmissionremote.cpp \
    src/torrentclient.cpp \
    src/transmissionCommands/sessionstats.cpp \
    src/transmissionCommands/torrentget.cpp \
    src/transmissionCommands/uploadtorrent.cpp \
    torrentdata.cpp \
    src/jsonrpcconnection.cpp

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    translations/*.ts \
    qml/pages/TorrentDelegate.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/TorrentPage.qml \
    qml/pages/TorrentFileDelegate.qml \
    qml/prettyFileSize.js \
    harbour-transmissionremote.desktop \
    rpm/harbour-transmissionremote.spec \
    rpm/harbour-transmissionremote.yaml \
    rpm/harbour-transmissionremote.changes.in \
    qml/harbour-transmissionremote.qml \
    qml/dialogs/AddTorrentDialog.qml

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-transmissionremote-de.ts

HEADERS += \
    src/rpcconnection.h \
    src/torrent.h \
    src/transmission.h \
    src/settings.h \
    src/torrentfile.h \
    src/torrentclient.h \
    src/transmissionCommands/sessionstats.h \
    src/transmissionCommands/torrentget.h \
    src/transmissionCommands/uploadtorrent.h \
    torrentdata.h \
    src/jsonrpcconnection.h

RESOURCES +=

