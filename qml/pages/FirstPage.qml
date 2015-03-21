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
import transmission 1.0


Page {
    id: page

    ListModel{

        id: testListModel
        Component.onCompleted: {
            testListModel.append(torrentOne)
            testListModel.append(torrentTwo)
            console.log("added");
        }
    }
    Transmission{
        id: transmission
        Component.onCompleted: {
            console.log("T: "  + torrents);
        }
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        contentHeight: column.height
        contentWidth: parent.width

        VerticalScrollDecorator{}

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Show Page 2")
                onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"))
            }
        }



        Column {
            id: column

            anchors.topMargin: pageHeader.heigth
            anchors{
                left: parent.left
                right: parent.right
            }

            spacing: Theme.paddingLarge
            height: childrenRect.height


            PageHeader {
                id: pageHeader
                title: qsTr("UI Template")
            }


            Torrent{
                id:torrentOne
                percentage: 60
                name: "test"
            }
            Torrent{
                id:torrentTwo
                percentage: 50
                name: "testtwo"
            }

            Row{
                anchors {
                    leftMargin: Theme.paddingMedium
                    rightMargin: Theme.paddingMedium
                    left: parent.left
                    right: parent.right
                    top: pageHeader.bottom
                }
                ListView{
                    width: parent.width
                    height: contentHeight

                    model: transmission.torrents
                    delegate: TorrentDelegate{}

                }
            }
        }
    }
}
