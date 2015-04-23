import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.transmissionremote 1.0

Item {
    id:root
    property ListView listView: ListView.view
    width: listView.width

    height: contentItem.height
    BackgroundItem{
        id: contentItem
        width: parent.width
        Switch{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            id: wantedSwitch
            checked: model.wanted
        }
        Label{
            anchors.left: wantedSwitch.right
            anchors.right: parent.right
            id: nameLabel
            elide: Text.ElideMiddle
            text: model.fileName
        }
        ProgressBar{
            anchors.left: wantedSwitch.right
            anchors.right: parent.right
            anchors.top: nameLabel.verticalCenter

            maximumValue: 100
            value: model.percentage
        }
    }
}
