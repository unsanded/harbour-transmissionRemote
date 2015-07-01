import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.transmissionremote 1.0

Dialog {

    property string type
    property QtObject client
    property Settings st

    Column{
        DialogHeader{
            acceptText: "add client"
            cancelText: "cancel"
        }
        id: settings
        width: parent.width
        height: parent.height

        ComboBox{
            label: "type"
            id: typeSelect
            width: parent.width
            visible: (client==null)
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
            label: "name"
            placeholderText: "name"
        }
        TextField {
            id: urlFIeld
            inputMethodHints: Qt.ImhUrlCharactersOnly
            width: parent.width
            label: "host"
            text: settings.transmissionHost
            Component.onCompleted: {
                if(text.length < 3)
                    text="http://192.168.1.10:9091/transmission/rpc"
                select(7,19)
            }
        }
        TextField{
            id: usernameField
            width: parent.width
            label: "username"
            placeholderText: "username"
        }
        TextField{
            id: passwordField
            label: "password"
            placeholderText: "password"
            width: parent.width
        }
    }
    onAccepted: {
        if (client == null){
            var type = typeSelect.currentItem.text;
            client=st.addClient(type, nameField.text, urlFIeld.text, usernameField.text, passwordField.text);
        }
        else
        {
            client.name = nameField.text
            client.url  = urlFIeld.text
            client.username = usernameField.text
            client.password = passwordField.text
        }
    }
}
