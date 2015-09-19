/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.transmissionremote 2.0

Dialog {
    id: page

    property Settings settings




    onAccepted: {
        console.log('accepted settings ')
        settings.saveClients();
    }

    Column {
        id: settingsColumn

        anchors.left:  parent.left + Theme.horizontalPageMargin
        anchors.right: parent.right - Theme.horizontalPageMargin
        spacing: 10
        height: 400
        DialogHeader{
            acceptText: "save"
            cancelText: "cancel"
        }
        Button{
            anchors.horizontalCenter: parent.horizontalCenter
            text:"add client"
            width: parent.width
            onClicked: {
            pageStack.push
               (Qt.resolvedUrl("ClientSettingsDialog.qml"),
                   {
                        "st": st,
                        "client": null
                    })
            }
        }
    }
    ListView{
        id:clientList
        model: settings.clients
        width: parent.width
        height: contentHeight
        anchors.top: settingsColumn.bottom

        delegate: Component{
            BackgroundItem{
                width: clientList.width
                height: clientName.height + clientType.height
                Column{
                    Label{
                        id: clientName
                        text: model.name
                        font.pixelSize: Theme.fontSizeLarge
                        width: parent.width
                    }
                    Label{
                        id: clientType
                        text: model.type
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeSmall
                        width: parent.width
                    }

                    ContextMenu{
                        MenuItem{
                            text: "delete";
                            onClicked: {
                                deleteRemorse.visible=true
                                deleteRemorse.execute("deleting connection", function() {settings.removeClient(model.name); } )
                            }
                        }
                    }
                    RemorseItem{
                        id: deleteRemorse;
                    }
               }
                onClicked: {

                pageStack.push
                   (Qt.resolvedUrl("ClientSettingsDialog.qml"),
                       {
                            "st": settings,
                            "client": model
                        })
                }
            }
        }
    }
}

