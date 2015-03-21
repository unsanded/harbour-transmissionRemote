import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: root

    property ListView listView: ListView.view
    width: listView.width
    height: contentItem.height + Theme.paddingMedium

    BackgroundItem{
        id:contentItem
        width: root.width
        Column{
            width: root.width
            Row{
                width: root.width
                Label{
                    color: Theme.primaryColor
                    text: model.name + "(" + model.fileCount + ")"
                }
            }
            Row{
                width: root.width
                ProgressBar{
                    value: model.percentage
                    width: root.width
                    maximumValue: 100
                }
            }
        }
    }
}
