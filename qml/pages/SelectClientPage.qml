import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.transmissionremote 2.0
import "../dialogs"
import "../prettyFileSize.js" as Pretty
Page {
    id: page

    property Settings settings

    Timer{
        repeat: true;
        interval: 1000
        running: autoUpdateSwitch.checked && page.visible
        onTriggered: {
            for (var c in settings.clients)
                settings.clients[c].updateStats();
        }
    }

    SilicaListView{
        id: clientList
        model: settings.clients
        anchors.fill: parent
        leftMargin: Theme.horizontalPageMargin
        topMargin: Theme.paddingLarge
        PullDownMenu {
            TextSwitch{
                id:autoUpdateSwitch
                text: qsTr("auto update");
                onCheckedChanged: {
                    settings.autoUpdateInClientSelect = checked
                }
            }

            MenuItem {
                text: qsTr("settings")
                onClicked:{
                    pageStack.push
                   (
                       Qt.resolvedUrl("../dialogs/SettingsDialog.qml"),
                       {"settings": settings}
                   )
                }
            }
            MenuItem {
                text: qsTr("refresh")
                onClicked: {
                    for (var c in settings.clients){
                        settings.clients[c].updateStats();
                    }
                }
            }//MenuItem
        }//pulldownmenu
        delegate: BackgroundItem{
            id: client
            width: parent.width

            onClicked: {
                pageStack.push(
                            Qt.resolvedUrl("FirstPage.qml"),
                            {
                              "client": modelData,
                              "settings": settings
                            }
                        );
                }
            Column{
                width: parent.width
                Label{
                    text: modelData.name
                    width: parent.width
                    font.pointSize: Theme.fontSizeExtraLarge
                }
                Label{
                    text: modelData.type
                    width: parent.width
                    color: Theme.secondaryColor
                }
                Label{
                    text: qsTr("up: ") + Pretty.prettyFileSize(modelData.upSpeed) + "/s";
                    width: parent.width
                    color: Theme.secondaryColor
                }
                Label{
                    text: qsTr("down: ") + Pretty.prettyFileSize(modelData.downSpeed) + "/s";
                    width: parent.width
                    color: Theme.secondaryColor
                }
            }
        }
    }
}
