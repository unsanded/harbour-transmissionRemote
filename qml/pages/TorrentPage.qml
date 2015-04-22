import QtQuick 2.0
import QtQuick 2.0
import Sailfish.Silica 1.0
import transmission 1.0


Page {
    id: page
    property Torrent torrent

    Component.onCompleted: {
        torrent.fullUpdate()
    }

    SilicaListView{
        id: fileView
        model: torrent.files
        width: parent.width
        height: parent.height

        delegate: TorrentFileDelegate{}

        VerticalScrollDecorator{}

        PullDownMenu{
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
