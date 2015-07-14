import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.transmissionremote 1.0

Dialog {
    property QtObject client
    property alias location: locationInput.text

    property string torrentFile

    onAccepted: {
        console.log("uploading torrent ", torrentFile)
        client.uploadTorrent(torrentFile, autoStartSwitch.checked, locationInput.text);
    }
    Component.onCompleted: {
        client.updateTorrents([], []);
    }

    Column{
        id: settingsColumn
        VerticalScrollDecorator{}
        width: parent.width
        spacing: 10;
        DialogHeader{
            id: header
            cancelText: qsTr("cancel")
            acceptText: qsTr("add torrent")
        }
        Row{
            height: autoStartSwitch.height
            width: parent.width

            TextSwitch{
                id: autoStartSwitch
                checked: true
                text:"start"
                description: "start downloading immediately"
                width: parent.width
            }
        }
        TextField{
            id: locationInput
            label:  qsTr("download location")
            width: parent.width
            placeholderText: qsTr("path to download to")
        }
    }
    ListView{
        width: parent.width
        model: client.saveLocations
        height: contentHeight
        anchors.top: settingsColumn.bottom

        delegate: BackgroundItem{
            height: locationSuggestion.height + 10
            onClicked: {
                locationInput.text = modelData
            }
            Label{
                id: locationSuggestion
                text: modelData
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                elide: Text.ElideMiddle
            }
        }
    }
}
