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

    Component.onCompleted: motorStatusDialog.open()

    property variant motorConfigWin;

    Dialog{
        id: motorStatusDialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 400
        height: 300
        parent: Overlay.overlay
        modal: Qt.ApplicationModal
        focus: true
        //closePolicy: "CloseOnEscape"
        //Component.onCompleted: encoder.getMotorStatus()
        //onOpened: encoder.getMotorStatus()
        //onFocusChanged: encoder.getMotorStatus()
        //onAccepted: motorStatusDialog.close()
        onVisibleChanged: {
            if(visible){
                encoder.getMotorStatus()
            }
        }

        BusyIndicator {
            id: busyIndicator
            x: (parent.width - width) / 2
            y: 20

            width: 50
            height: 50
            running: true
            //onRunningChanged: encoder.getMotorStatus()

        }

        Button {
            id: doneButton
            text: "DONE"
            enabled: false
            onClicked: motorStatusDialog.close()
        }

    }



    /*Popup {
        id: popup
        x: Math.round((parent.width - width) / 2)
        y: Math.round((parent.height - height) / 2)
        width: 400
        height: 200
        modal: true
        focus: true
        closePolicy: "NoAutoClose"
        onAboutToShow: encoder.getMotorStatus()

        BusyIndicator {
            id: busyIndicator
            parent: popup
            x: Math.round((parent.width - width) / 2)
            y: Math.round((parent.height - height) / 2)
            antialiasing: true
            running: true
        }
    }*/

    Encoder {
        id: encoder

        onUpdateEncoder: encoderText.text = qsTr("GRADI ENCODER: " + degrees)
        onClosePopup: {
            console.log("PopUp close signal")
            //motorStatusDialog.done(Dialog.Accepted)
            ditoSButton.palette.button = "green"
            doneButton.enabled = true
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
        palette.button: "red"
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
    }

    RoundButton {
        id: nastroSButton
        x: 230
        y: 320
        text: ""
        antialiasing: true
        hoverEnabled: false
        enabled: false
    }

    RoundButton {
        id: corpoSButton
        x: 310
        y: 320
        text: ""
        antialiasing: true
        hoverEnabled: false
        enabled: false
    }

    RoundButton {
        id: manineSButton
        x: 390
        y: 320
        text: ""
        antialiasing: true
        hoverEnabled: false
        enabled: false
    }

    RoundButton {
        id: chiusuraSButton
        x: 470
        y: 320
        text: ""
        antialiasing: true
        hoverEnabled: false
        enabled: false
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
            motorConfigWin = component.createObject(mainWindow);
            motorConfigWin.show();
        }*/
    }

    Button {
        id: fasiButton
        x: 660
        y: 241
        text: qsTr("FASI")
    }

}
