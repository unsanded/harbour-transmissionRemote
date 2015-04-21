import QtQuick 2.0
import Sailfish.Silica 1.0
import transmission 1.0

Item {
    id:root
    property ListView listView: ListView.view
    width: listView.width

        height: contentItem.height
        BackgroundItem{
        id: contentItem
        width: parent.width
        Column{

            width: parent.width
            Row{
                width: parent.width
                Switch{
                    checked: model.wanted
                }
                Label{
                    id: nameLabel
                    width: parent.width
                    elide: Text.ElideMiddle
                    text: model.fileName
                }
            }
            Row{
                width: parent.width
                ProgressBar{
                    valueText: model.haveBytes + "/" + model.completedBytes
                    width: parent.width
                    maximumValue: 100
                    value: model.percentage
                }
            }
        }
    }
}
