import QtQuick 2.0
import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.transmissionremote 1.0


Page {
    id: page
    property Torrent torrent
    property QtObject client

    Component.onCompleted: {
        torrent.fullUpdate()
    }
    Timer{
        repeat: true
        interval: 1000
        running: autoUpdateSwitch.checked
        onTriggered: {
            torrent.fullUpdate();
        }
    }

    SilicaListView{
        id: fileView
        model: torrent.files
        width: parent.width
        height: parent.height

        delegate: TorrentFileDelegate{}

        VerticalScrollDecorator{}

        PullDownMenu{
            TextSwitch{
                id: autoUpdateSwitch
            }
            MenuItem{
                text: "move data"
                onClicked: {
                    pageStack.push
                   (
                       Qt.resolvedUrl("../dialogs/MoveTorrentDialog.qml"),
                       {
                                    "torrent": torrent,
                                    "client": client
                                }
                   )
                }
            }

            MenuItem{
                text:qsTr("refresh")
                onClicked: {
                    torrent.fullUpdate()
                }
            }
        }

        header:PageHeader{
            id:header
            title: page.torrent.name
            description: torrent.percentage + "%"
        }
    }
}
