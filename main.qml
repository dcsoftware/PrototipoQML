import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.VirtualKeyboard 2.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import Encoder 1.0
import PigpioCommunication 1.0
import Motors 1.0


Window {
    id: mainWindow
    visible: true
    width: 800
    height: 480

    PigpioCommunication {
        id:picom
    }

    Encoder {
        id: encoder

        onUpdateEncoder: encoderText.text = qsTr("GRADI ENCODER: " + degrees)
        onClosePopup: {
            //ditoSButton.palette.button = "green"
        }
        onUpdateMotorStatus: {
            switch(motor){
            case 0x00:
                status ? ditoSButton.palette.button = "green" : ditoSButton.palette.button = "red"
                break;
            case 0x01:
                status ? corpoSButton.palette.button = "green" : corpoSButton.palette.button = "red"
                break;
            case 0x02:
                status ? manineSButton.palette.button = "green" : manineSButton.palette.button = "red"
                break;
            case 0x03:
                status ? chiusuraSButton.palette.button = "green" : chiusuraSButton.palette.button = "red"
                break;
            case 0x04:
                status ? lunettaSButton.palette.button = "green" : lunettaSButton.palette.button = "red"
                break;
            case 0x05:
                status ? nastroSButton.palette.button = "green" : nastroSButton.palette.button = "red"
                break;
            }
        }
    }

    Motors{
        id: motor
    }

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

    Flickable {
        id: flickable
        anchors.fill: parent

        Pane {
            id: mainPane
            anchors.fill: parent
            /*Component.onCompleted: {
                console.log("GetStatus Call")
                i2ccom.getAllStatus()
            }*/

            Button {
                id: quitButton
                x: 648
                y: 388
                text: qsTr("QUIT")
                onClicked: close();
            }

            TextField {
                id: encoderText
                x: 513
                y: 28
                width: 235
                height: 40
                text: "GRADI ENCODER: "
                enabled: false
            }

            Button {
                id: startEncButton
                x: 28
                y: 28
                text: qsTr("START")
                onClicked: encoder.startTimer();
            }

            Button {
                id: stopEncButton
                x: 238
                y: 28
                text: qsTr("STOP")
                onClicked: encoder.stopTimer();
            }

            Button {
                id: resetEncButton
                x: 648
                y: 318
                text: qsTr("RESET")
                onClicked: encoder.resetTimer();
            }

            RoundButton {
                id: ditoSButton
                x: 58
                y: 308
                text: ""
                antialiasing: true
                hoverEnabled: false
                //enabled: false
                //palette.button: "yellow"
                onClicked: picom.getStatus(0x00)
            }

            Label {
                id: label
                x: 58
                y: 373
                width: 40
                text: qsTr("DITO")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            RoundButton {
                id: lunettaSButton
                x: 138
                y: 308
                text: ""
                antialiasing: true
                hoverEnabled: false
                //enabled: false
                //palette.button: "yellow"
                onClicked: picom.getConfig(0x00)

            }

            RoundButton {
                id: nastroSButton
                x: 218
                y: 308
                text: ""
                antialiasing: true
                hoverEnabled: false
                //enabled: false
                //palette.button: "yellow"
                onClicked: picom.getPosition(0x00)

            }

            RoundButton {
                id: corpoSButton
                x: 298
                y: 308
                text: ""
                antialiasing: true
                hoverEnabled: false
                enabled: false
                //palette.button: "yellow"
            }

            RoundButton {
                id: manineSButton
                x: 378
                y: 308
                text: ""
                antialiasing: true
                hoverEnabled: false
                enabled: false
                //palette.button: "yellow"
            }

            RoundButton {
                id: chiusuraSButton
                x: 458
                y: 308
                text: ""
                antialiasing: true
                hoverEnabled: false
                enabled: false
                //palette.button: "yellow"
            }

            Label {
                id: label1
                x: 121
                y: 373
                text: qsTr("LUNETTA")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 13
            }

            Label {
                id: label2
                x: 205
                y: 373
                text: qsTr("NASTRO")
                font.pointSize: 13
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: label3
                x: 289
                y: 373
                text: qsTr("CORPO")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 13
            }

            Label {
                id: label4
                x: 365
                y: 373
                text: qsTr("MANINE")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 13
            }

            Label {
                id: label5
                x: 437
                y: 373
                text: qsTr("CHIUSURA")
                font.pointSize: 13
            }

            Button {
                id: manualButton
                x: 648
                y: 151
                text: qsTr("MANUALE")
                onClicked: manualPane.visible = true

            }

            Button {
                id: fasiButton
                x: 648
                y: 229
                text: qsTr("FASI")

            }

            Button {
                id: getStatusButton
                x: 28
                y: 130
                text: qsTr("GET STATUS")
                onClicked: i2ccom.getAllStatus()
            }
        }

        Pane {
            id: motorConfigPane
            anchors.fill: parent
            visible: false

        }

        Pane {
            id: phasesConfigPane
            anchors.fill: parent
            visible: false
        }

        Pane {
            id: manualPane
            anchors.fill: parent
            visible: false

            onVisibleChanged: i2ccom.getAllPosition()

            TextField {
                id: encoderText2
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
                    id: ditoLabel
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
                    highlighted: false
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 320
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
                    id: ditoPosLabel
                    width: 150
                    height: 30
                    text: qsTr("Posizione:")
                    anchors.left: ditoPos2Button.right
                    anchors.leftMargin: 40
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
            }

            Row {
                id: row1
                x: 50
                y: 180
                width: 700
                height: 60
                Label {
                    id: corpoLabel
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
                    anchors.rightMargin: 320
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

                Label {
                    id: corpoPosLabel
                    width: 150
                    height: 30
                    text: qsTr("Posizione:")
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: corpoPos2Button.right
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.leftMargin: 40
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
                    id: manineLabel
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
                    anchors.rightMargin: 320
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

                Label {
                    id: maninePosLabel
                    width: 150
                    height: 30
                    text: qsTr("Posizione:")
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: maninePos2Button.right
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.leftMargin: 40
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
                    id: chiusuraLabel
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
                    anchors.rightMargin: 320
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

                Label {
                    id: chiusuraPosLabel
                    width: 150
                    height: 30
                    text: qsTr("Posizione:")
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: chiusuraPos2Button.right
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.leftMargin: 40
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
                    id: lunettaLabel
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
                    anchors.rightMargin: 320
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

                Label {
                    id: lunettaPosLabel
                    width: 150
                    height: 30
                    text: qsTr("Posizione:")
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: lunettaPos2Button.right
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.leftMargin: 40
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
                    id: nastroLabel
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
                    anchors.rightMargin: 320
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

                Label {
                    id: nastroPosLabel
                    width: 150
                    height: 30
                    text: qsTr("Posizione:")
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: lunettaPos2Button1.right
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.leftMargin: 40
                }
                spacing: 2
            }
        }
    }
}
