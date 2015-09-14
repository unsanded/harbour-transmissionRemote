import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.transmissionremote 1.0

Dialog {

    property string type
    property QtObject client
    property Settings st

    Component.onCompleted: {
        console.log(client)
    }

    Column{
        DialogHeader{
            acceptText: (client==null) ? qsTr("add client"): qsTr("update client")
            cancelText: qsTr("cancel")
        }
        id: settings
        width: parent.width
        height: parent.height

        ComboBox{
            label: qsTr("type")
            id: typeSelect
            width: parent.width
            enabled: (client==null || client == undefined)
            menu: ContextMenu{

                MenuItem{
                    text: "transmission"
                }
                MenuItem{
                    text: "rtorrent"
                }
            }
        }

        TextField {
            id: nameField
            width: parent.width
            label: qsTr( "name")
            placeholderText: qsTr("name")
            text:client.name
            validator: RegExpValidator{
                regExp: /.+/
            }
        }
        TextField {
            id: urlFIeld
            inputMethodHints: Qt.ImhUrlCharactersOnly
            width: parent.width
            label: qsTr("host")
            text: client.url
            Component.onCompleted: {
                if(text.length < 3){
                    text="http://192.168.1.10:9091/transmission/rpc"
                    select(15,19)
                }

            }
        }
        TextField{
            id: usernameField
            inputMethodHints: Qt.ImhNoAutoUppercase + Qt.ImhNoPredictiveText
            width: parent.width
            label: "username"
            placeholderText: "username"
            text: client.username
        }
        TextField{
            id: passwordField
            inputMethodHints: Qt.ImhNoAutoUppercase + Qt.ImhNoPredictiveText
            label: "password"
            placeholderText: "password"
            width: parent.width
            text: client.password
        }
    }
    onAccepted: {
        if (client == null){
            var type = typeSelect.currentItem.text;
            client=st.addClient(type, nameField.text, urlFIeld.text, usernameField.text.trim(), passwordField.text.trim());
        }
        else
        {
            client.url  = urlFIeld.text
            client.username = usernameField.text.trim()
            client.password = passwordField.text.trim()
        }
    }
}
