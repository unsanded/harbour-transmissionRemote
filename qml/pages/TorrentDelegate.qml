import QtQuick 2.0
import Sailfish.Silica 1.0
import transmission 1.0

Item {
    id: root
    property ListView listView: ListView.view
    property Transmission transmission

    Component.onCompleted: {
        console.log(listView)
        console.log(parent)
    }

    signal onClicked
    width: listView.width
    height: 100
    BackgroundItem{
        id:contentItem
        width: root.width

        onClicked:     {
                          pageStack.push(
                           Qt.resolvedUrl("TorrentPage.qml"),
                           {"torrent": transmission.getTorrent(model.id)}
                          )
                        }

        Column{
            width: root.width
            Row{
                width: root.width
                Label{
                    color: Theme.primaryColor
                    text: model.name
                }
            }
            Row{
                width: root.width
                ProgressBar{
                    value: model.percentage
                    width: root.width
                    maximumValue: 100
                }
            }
        }
    }
}
