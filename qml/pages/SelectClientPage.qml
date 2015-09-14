import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.transmissionremote 1.0
import "../dialogs"
import "../prettyFileSize.js" as Pretty
Page {
    id: page

    property Settings settings

    SilicaListView{
        id: clientList
        model: settings.clients
        anchors.fill: parent
        PullDownMenu {
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
                    for (client in settings.clients)
                            client.updateStats();
                }
            }//MenuItem
        }//pulldownmenu
        delegate: BackgroundItem{
            id: client
            width: parent.width
            height: 200

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
                    font.pointSize: Theme.fontSizeLarge
                }
                Label{
                    text: qsTr("up: ") + Pretty.prettyFileSize(modelData.upSpeed) + "/s";
                    width: parent.width
                }
                Label{
                    text: qsTr("down: ") + Pretty.prettyFileSize(modelData.downSpeed) + "/s";
                    width: parent.width
                }
            }
        }
    }
}
