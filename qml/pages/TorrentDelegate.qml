import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.transmissionremote 2.0

Item {
    id: root
    property ListView listView: ListView.view
    property QtObject client
    property Torrent torrent: modelData

    signal onClicked
    width: listView.width
    height: 100
    BackgroundItem{
        id:contentItem
        width: root.width

        onClicked:     {
                          pageStack.push(
                           Qt.resolvedUrl("TorrentPage.qml"),
                           {

                                  "torrent": torrent,
                                  "client": client
                              }
                          )
                        }

        Label{
            width: parent.width
            id: nameLabel
            color: Theme.primaryColor
            text: model.name
        }
        ProgressBar{
            value: model.percentage
            anchors.top: nameLabel.verticalCenter
            width: parent.width
            maximumValue: 100
        }
    }
}
