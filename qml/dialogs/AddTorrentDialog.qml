import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.transmissionremote 1.0

Dialog {
    id: root
    property Settings settings
    property alias location: locationInput.text
    property QtObject client
    property string torrentFile


    onAccepted: {
        console.log("uploading torrent ", torrentFile)
        client = settings.getClient(clientName)
        client.uploadTorrent(torrentFile, autoStartSwitch.checked, locationInput.text);
    }
    Component.onCompleted: {
        client.updateTorrents([], []);
    }

    Column{
        id: settingsColumn
        width: parent.width
        spacing: 10
        DialogHeader{
            id: header
            cancelText: qsTr("cancel")
            acceptText: qsTr("add torrent")
        }
        ComboBox{
            id:clientSelect
            width: parent.width
            menu: ContextMenu{
                Repeater{
                    model: settings.clients
                    MenuItem{
                        property QtObject client : modelData
                        text: client.name
                        onClicked: {
                            root.client = this.client
                        }
                    }
                }
            }
        }

        TextSwitch{
            id: autoStartSwitch
            checked: true
            text:"start"
            description: "start downloading immediately"
            width: parent.width
        }
        TextField{
            id: locationInput
            label:  qsTr("download location")
            width: parent.width
            placeholderText: qsTr("path to download to")
        }
    }
    ListView{
        id: locationSuggestionList
        model: client.saveLocations

        width: parent.width
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
                font.pointSize: 16
            }
        }
    }
}
