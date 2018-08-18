import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.VirtualKeyboard 2.3
import QtQuick.Controls 2.4
import Encoder 1.0

Window {
    id: phaseConfigWindow
    visible: true
    width: 800
    height: 480

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: mainWindow.height
        width: mainWindow.width
        active: false

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: mainWindow.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

    TextField {
        id: encoderText
        x: 300
        y: 28
        text: qsTr("Text Field")
        font.pointSize: 16
        horizontalAlignment: Text.AlignHCenter
    }

    Row {
        id: row
        x: 50
        y: 120
        width: 700
        height: 60
        spacing: 2

        Label {
            id: label
            x: 0
            y: 13
            height: 30
            text: qsTr("DITO")
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 16
        }

        Button {
            id: ditoPos1Button
            x: 200
            y: 2
            width: 80
            height: 30
            text: qsTr("POS 1")
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 280
            transformOrigin: Item.Center
        }

        Button {
            id: ditoPos2Button
            y: 2
            width: 80
            height: 30
            text: qsTr("POS 2")
            anchors.left: ditoPos1Button.right
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter
            transformOrigin: Item.Center
        }

        Label {
            id: label6
            height: 30
            text: qsTr("Label")
            anchors.left: ditoPos2Button.right
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 14
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Row {
        id: row1
        x: 50
        y: 180
        width: 700
        height: 60
        Label {
            id: label1
            x: 0
            y: 13
            height: 30
            text: qsTr("COPRO MANINE")
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 16
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            id: corpoPos1Button
            x: 200
            y: 2
            width: 80
            height: 30
            text: qsTr("POS 1")
            anchors.verticalCenter: parent.verticalCenter
            transformOrigin: Item.Center
            anchors.rightMargin: 280
            anchors.right: parent.right
        }

        Button {
            id: corpoPos2Button
            y: 2
            width: 80
            height: 30
            text: qsTr("POS 2")
            anchors.left: corpoPos1Button.right
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter
            transformOrigin: Item.Center
        }
        spacing: 2
    }

    Row {
        id: row2
        x: 50
        y: 240
        width: 700
        height: 60
        Label {
            id: label2
            x: 0
            y: 13
            height: 30
            text: qsTr("MANINE")
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 16
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            id: maninePos1Button
            x: 200
            y: 2
            width: 80
            height: 30
            text: qsTr("POS 1")
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 280
            transformOrigin: Item.Center
            anchors.right: parent.right
        }

        Button {
            id: maninePos2Button
            y: 2
            width: 80
            height: 30
            text: qsTr("POS 2")
            anchors.left: maninePos1Button.right
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter
            transformOrigin: Item.Center
        }
        spacing: 2
    }

    Row {
        id: row3
        x: 50
        y: 300
        width: 700
        height: 60
        Label {
            id: label3
            x: 0
            y: 13
            height: 30
            text: qsTr("CHIUSURA MANINE")
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 16
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            id: chiusuraPos1Button
            x: 200
            y: 2
            width: 80
            height: 30
            text: qsTr("POS 1")
            anchors.verticalCenter: parent.verticalCenter
            transformOrigin: Item.Center
            anchors.rightMargin: 280
            anchors.right: parent.right
        }

        Button {
            id: chiusuraPos2Button
            y: 2
            width: 80
            height: 30
            text: qsTr("POS 2")
            anchors.left: chiusuraPos1Button.right
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter
            transformOrigin: Item.Center
        }
        spacing: 2
    }

    Row {
        id: row4
        x: 50
        y: 360
        width: 700
        height: 60
        Label {
            id: label4
            x: 0
            y: 13
            height: 30
            text: qsTr("LUNETTA")
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 16
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            id: lunettaPos1Button
            x: 200
            y: 2
            width: 80
            height: 30
            text: qsTr("POS 1")
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 280
            transformOrigin: Item.Center
            anchors.right: parent.right
        }

        Button {
            id: lunettaPos2Button
            y: 2
            width: 80
            height: 30
            text: qsTr("POS 2")
            anchors.left: lunettaPos1Button.right
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter
            transformOrigin: Item.Center
        }
        spacing: 2
    }

    Row {
        id: row5
        x: 50
        y: 420
        width: 700
        height: 60
        Label {
            id: label5
            x: 0
            y: 13
            height: 30
            text: qsTr("NASTRO")
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 16
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Button {
            id: nastroPos1Button1
            x: 200
            y: 2
            width: 80
            height: 30
            text: qsTr("PASSO")
            anchors.verticalCenter: parent.verticalCenter
            transformOrigin: Item.Center
            anchors.rightMargin: 280
            anchors.right: parent.right
        }

        Button {
            id: lunettaPos2Button1
            y: 2
            width: 80
            height: 30
            text: qsTr("AVANZ")
            anchors.left: nastroPos1Button1.right
            anchors.leftMargin: 40
            anchors.verticalCenter: parent.verticalCenter
            transformOrigin: Item.Center
        }
        spacing: 2
    }
}

/*##^## Designer {
    D{i:19;anchors_width:80;anchors_x:200}D{i:23;anchors_width:80;anchors_x:200}D{i:27;anchors_width:80;anchors_x:200}
D{i:31;anchors_width:80;anchors_x:200}D{i:35;anchors_width:80;anchors_x:200}D{i:39;anchors_width:80;anchors_x:200}
}
 ##^##*/
