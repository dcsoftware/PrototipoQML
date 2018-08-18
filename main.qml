import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.VirtualKeyboard 2.3
import QtQuick.Controls 2.4
import Encoder 1.0
import I2CCom 1.0

Window {
    id: mainWindow
    visible: true
    width: 800
    height: 480

    property variant motorConfigWindow;
    property variant phaseConfigWindow

    Encoder {
        id: encoder

        onUpdateEncoder: encoderText.text = qsTr("GRADI ENCODER: " + degrees)
        onClosePopup: {
            //ditoSButton.palette.button = "green"
        }
        onUpdateMotorStatus: {
            switch(motor){
            case 0x01:
                status ? ditoSButton.palette.button = "green" : ditoSButton.palette.button = "red"
                break;
            case 0x02:
                status ? corpoSButton.palette.button = "green" : corpoSButton.palette.button = "red"
                break;
            case 0x03:
                status ? manineSButton.palette.button = "green" : manineSButton.palette.button = "red"
                break;
            case 0x04:
                status ? chiusuraSButton.palette.button = "green" : chiusuraSButton.palette.button = "red"
                break;
            case 0x05:
                status ? lunettaSButton.palette.button = "green" : lunettaSButton.palette.button = "red"
                break;
            case 0x06:
                status ? nastroSButton.palette.button = "green" : nastroSButton.palette.button = "red"
                break;
            }
        }
    }

    I2CCom {
        id: i2ccom
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

    Button {
        id: quitButton
        x: 660
        y: 400
        text: qsTr("QUIT")
        onClicked: close();
    }

    TextField {
        id: encoderText
        x: 525
        y: 40
        width: 235
        height: 40
        text: "GRADI ENCODER: "
        enabled: false
    }

    Button {
        id: startEncButton
        x: 40
        y: 40
        text: qsTr("START")
        onClicked: encoder.startTimer();
    }

    Button {
        id: stopEncButton
        x: 250
        y: 40
        text: qsTr("STOP")
        onClicked: encoder.stopTimer();
    }

    Button {
        id: resetEncButton
        x: 660
        y: 330
        text: qsTr("RESET")
        onClicked: encoder.resetTimer();
    }

    RoundButton {
        id: ditoSButton
        x: 70
        y: 320
        text: ""
        antialiasing: true
        display: AbstractButton.IconOnly
        hoverEnabled: false
        enabled: false
        palette.button: "yellow"
    }

    Label {
        id: label
        x: 70
        y: 385
        width: 40
        text: qsTr("DITO")
        font.pointSize: 13
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    RoundButton {
        id: lunettaSButton
        x: 150
        y: 320
        text: ""
        antialiasing: true
        hoverEnabled: false
        enabled: false
        palette.button: "yellow"
    }

    RoundButton {
        id: nastroSButton
        x: 230
        y: 320
        text: ""
        antialiasing: true
        hoverEnabled: false
        enabled: false
        palette.button: "yellow"
    }

    RoundButton {
        id: corpoSButton
        x: 310
        y: 320
        text: ""
        antialiasing: true
        hoverEnabled: false
        enabled: false
        palette.button: "yellow"
    }

    RoundButton {
        id: manineSButton
        x: 390
        y: 320
        text: ""
        antialiasing: true
        hoverEnabled: false
        enabled: false
        palette.button: "yellow"
    }

    RoundButton {
        id: chiusuraSButton
        x: 470
        y: 320
        text: ""
        antialiasing: true
        hoverEnabled: false
        enabled: false
        palette.button: "yellow"
    }

    Label {
        id: label1
        x: 133
        y: 385
        text: qsTr("LUNETTA")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 13
    }

    Label {
        id: label2
        x: 217
        y: 385
        text: qsTr("NASTRO")
        font.pointSize: 13
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Label {
        id: label3
        x: 301
        y: 385
        text: qsTr("CORPO")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 13
    }

    Label {
        id: label4
        x: 377
        y: 385
        text: qsTr("MANINE")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 13
    }

    Label {
        id: label5
        x: 449
        y: 385
        text: qsTr("CHIUSURA")
        font.pointSize: 13
    }

    Button {
        id: manualButton
        x: 660
        y: 163
        text: qsTr("MANUALE")
        onClicked: motorStatusDialog.open()

        /*{
            var component = Qt.createComponent("MotorConfigWindow.qml");
            motorConfigWindow = component.createObject(mainWindow);
            motorConfigWindow.show();
        }*/
    }

    Button {
        id: fasiButton
        x: 660
        y: 241
        text: qsTr("FASI")
        //onClicked:
        /*{
            var component = Qt.createComponent("PhaseConfigWindow.qml");
            phaseConfigWindow = component.createObject(mainWindow);
            phaseConfigWindow.show();
        }*/
    }

}
