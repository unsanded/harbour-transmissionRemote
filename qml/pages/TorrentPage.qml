import QtQuick 2.0
import QtQuick 2.0
import Sailfish.Silica 1.0
import transmission 1.0


Page {
    id: page
    property Torrent torrent



    Column{
        VerticalScrollDecorator{}
        width: parent.width
        PageHeader{
            id:nameLabel
            title: page.torrent.name
            Label{
                anchors.right: parent.right
                anchors.top: nameLabel.top
                id: percentageLabel
                color: Theme.secondaryColor
                text: torrent.percentage + "%"
            }
        }
        Button{
            id:updateButon
            text: "Update"
            onClicked: {
                torrent.fullUpdate();
            }
        }

        Row{
            width: parent.width

            ListView{
                id: fileView
                model: torrent.files
                width: parent.width
                height: contentHeight

                delegate: TorrentFileDelegate{}

            }
        }
    }
}
