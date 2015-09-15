import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.transmissionremote 2.0

Dialog {
    id: root
    property alias location: locationInput.text
    property Torrent torrent;
    property QtObject client;


    onAccepted: {
        torrent.moveData(location);
    }
    Component.onCompleted: {
    }

    Column{
        id: settingsColumn
        width: parent.width
        spacing: 10

        DialogHeader{
            id: header
            cancelText: qsTr("cancel")
            acceptText: qsTr("move data")
        }
        Label{
            width: parent.width
            elide: Text.ElideMiddle
            text: qsTr("from: ") + torrent.downloadDir
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
