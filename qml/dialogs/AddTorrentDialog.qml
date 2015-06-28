import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.transmissionremote 1.0

Dialog {
    property Transmission tm
    property alias location: locationInput.text

    property string torrentFile

    onAccepted: {
        console.log("uploading torrent ", torrentFile)
        tm.uploadTorrent(torrentFile, autoStartSwitch.checked, locationInput.text);
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
            height: autoStartSwitch
            width: parent.width
            Switch{
                id: autoStartSwitch
                checked: true
            }
            Label{
                text: qsTr("start downloading")
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
        model: tm.saveLocations
        height: contentHeight

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
                color: Theme.secondary
            }
        }
    }
}
