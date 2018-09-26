import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.VirtualKeyboard 2.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import Encoder 1.0
import PigpioCommunication 1.0
import Motors 1.0
import XmlReaderWriter 1.0


Window {
    id: mainWindow
    visible: true
    width: 800
    height: 480

    /*PigpioCommunication {
        id:picom
        onStatusUpdated: {
            switch(_motor){
            case 0x00:
                _status ? ditoSButton.palette.button = "green" : ditoSButton.palette.button = "red"
                _status ? ditoSButton.enabled = true : ditoSButton.enabled = false;
                _status ? ditoPos1Button.enabled = true : ditoPos1Button.enabled = false
                _status ? ditoPos2Button.enabled = true : ditoPos2Button.enabled = false
                break;
            case 0x01:
                _status ? corpoSButton.palette.button = "green" : corpoSButton.palette.button = "red"
                _status ? corpoSButton.enabled = true : corpoSButton.enabled = false;
                _status ? corpoPos1Button.enabled = true : corpoPos1Button.enabled = false
                _status ? corpoPos1Button.enabled = true : corpoPos1Button.enabled = false
                break;
            case 0x02:
                _status ? manineSButton.palette.button = "green" : manineSButton.palette.button = "red"
                _status ? manineSButton.enabled = true : manineSButton.enabled = false;
                _status ? maninePos1Button.enabled = true : maninePos1Button.enabled = false
                _status ? maninePos1Button.enabled = true : maninePos1Button.enabled = false
                break;
            case 0x03:
                _status ? chiusuraSButton.palette.button = "green" : chiusuraSButton.palette.button = "red"
                _status ? chiusuraSButton.enabled = true : chiusuraSButton.enabled = false;
                _status ? chiusuraPos1Button.enabled = true : chiusuraPos1Button.enabled = false
                break;
            case 0x04:
                _status ? lunettaSButton.palette.button = "green" : lunettaSButton.palette.button = "red"
                _status ? lunettaSButton.enabled = true : lunettaSButton.enabled = false;
                _status ? lunettaPos1Button.enabled = true : lunettaPos1Button.enabled = false
                break;
            case 0x05:
                _status ? nastroSButton.palette.button = "green" : nastroSButton.palette.button = "red"
                _status ? nastroSButton.enabled = true : nastroSButton.enabled = false;
                _status ? nastroPos1Button.enabled = true : nastroPos1Button.enabled = false
                break;
            }
            startEncButton.enabled = (ditoSButton.enabled && corpoSButton.enabled && manineSButton.enabled)
            stopEncButton.enabled = (ditoSButton.enabled && corpoSButton.enabled && manineSButton.enabled)

        }
    }*/

    XmlReaderWriter {
        id: xml
    }


    Encoder {
        id: encoder

        onUpdateEncoder: encoderText.text = qsTr("ENCODER: " + degrees)
        onUpdateTextArea: textArea.append(_str);
        onPosUpdated: {
            switch(_motor){
            case 0x00:
                mainDitoPoslabel.text = _pos
                manualPane.visible ? ditoPosLabel.text = _pos : null
                break;
            case 0x01:
                mainCorpoPoslabel.text = _pos
                manualPane.visible ? corpoPosLabel.text = _pos :null
                break;
            case 0x02:
                mainManPoslabel.text = _pos
                manualPane.visible ? maninePosLabel.text = _pos : null
                break;
            case 0x03:
                mainChiuPoslabel.text = _pos
                manualPane.visible ? chiusuraPosLabel.text = _pos : null
                break;
            case 0x04:
                mainLunePoslabel.text = _pos
                manualPane.visible ? lunettaPosLabel.text = _pos : null
                break;
            case 0x05:
                mainNasPoslabel.text = _pos
                manualPane.visible ? nastroPosLabel.text = _pos : null
                break;
            }
        }

        onStatusUpdated: {
            switch(_motor){
            case 0x00:
                _status ? ditoSButton.palette.button = "green" : ditoSButton.palette.button = "red"
                _status ? ditoSButton.enabled = true : ditoSButton.enabled = false;
                _status ? ditoPos1Button.enabled = true : ditoPos1Button.enabled = false
                break;
            case 0x01:
                _status ? corpoSButton.palette.button = "green" : corpoSButton.palette.button = "red"
                _status ? corpoSButton.enabled = true : corpoSButton.enabled = false;
                break;
            case 0x02:
                _status ? manineSButton.palette.button = "green" : manineSButton.palette.button = "red"
                _status ? manineSButton.enabled = true : manineSButton.enabled = false;
                break;
            case 0x03:
                _status ? chiusuraSButton.palette.button = "green" : chiusuraSButton.palette.button = "red"
                _status ? chiusuraSButton.enabled = true : chiusuraSButton.enabled = false;
                break;
            case 0x04:
                _status ? lunettaSButton.palette.button = "green" : lunettaSButton.palette.button = "red"
                _status ? lunettaSButton.enabled = true : lunettaSButton.enabled = false;
                break;
            case 0x05:
                _status ? nastroSButton.palette.button = "green" : nastroSButton.palette.button = "red"
                _status ? nastroSButton.enabled = true : nastroSButton.enabled = false;
                _status ? nastroPos1Button.enabled = true : nastroPos1Button.enabled = false
                _status ? nastroPos2Button.enabled = true : nastroPos2Button.enabled = false
                break;
            }
            startEncButton.enabled = (ditoSButton.enabled && corpoSButton.enabled && manineSButton.enabled)
            stopEncButton.enabled = (ditoSButton.enabled && corpoSButton.enabled && manineSButton.enabled)
            if(ditoSButton.enabled && corpoSButton.enabled && manineSButton.enabled)
            {
                //encoder.setSoftStop(0xAB)
            }
        }

        onStepModeUpdated: {
            switch (_motor) {
            case 0x00:
                stepCombo.currentIndex = _data
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
            antialiasing: true
            z: 1
            anchors.fill: parent
            Component.onCompleted: {
                console.log("FIRST RUN")
                //encoder.getStatus(0xAB);
                //encoder.setHomePos(0xAB)
                //encoder.setResetMotor(0xAB)
                encoder.firstRun()
            }

            Button {
                id: quitButton
                x: 648
                y: 388
                height: 50
                text: qsTr("ESCI")
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                font.pointSize: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                onClicked: close();
            }

            TextField {
                id: encoderText
                x: 550
                y: 20
                width: 230
                height: 50
                text: "ENCODER: "
                font.pointSize: 20
                anchors.right: parent.right
                anchors.rightMargin: 20
                enabled: false
            }

            Button {
                id: startEncButton
                x: 20
                y: 20
                width: 100
                height: 50
                text: qsTr("START")
                display: AbstractButton.TextOnly
                font.pointSize: 18
                font.bold: true
                enabled: false
                onClicked: encoder.startTimer();
            }

            Button {
                id: stopEncButton
                x: 150
                y: 20
                height: 50
                text: qsTr("STOP")
                display: AbstractButton.TextOnly
                font.bold: true
                font.pointSize: 20
                enabled: false
                onClicked: encoder.stopTimer();
            }

            Button {
                id: resetEncButton
                x: 300
                y: 20
                height: 50
                text: qsTr("RESET")
                display: AbstractButton.TextOnly
                font.bold: true
                font.pointSize: 20
                onClicked: {
                    encoder.resetTimer();
                    //encoder.getStatus(0xAB)
                    encoder.setResetMotor(0xAB)
                }
            }

            RoundButton {
                id: ditoSButton
                x: 40
                y: 300
                text: ""
                enabled: false
                antialiasing: true
                hoverEnabled: false
                //enabled: false
                //palette.button: "yellow"
                onClicked: {
                    ditoConfigPane.z = 1
                    ditoConfigPane.visible = true
                }
            }

            Label {
                id: label
                x: 15
                y: 350
                width: 90
                text: qsTr("DITO")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            RoundButton {
                id: lunettaSButton
                y: 300
                text: ""
                anchors.left: chiusuraSButton.right
                anchors.leftMargin: 60
                enabled: false
                antialiasing: true
                hoverEnabled: false
                //enabled: false
                //palette.button: "yellow"
                onClicked: encoder.getStatus(0x04)

            }

            RoundButton {
                id: nastroSButton
                y: 300
                text: ""
                anchors.left: lunettaSButton.right
                anchors.leftMargin: 60
                enabled: false
                antialiasing: true
                hoverEnabled: false
                //enabled: false
                //palette.button: "yellow"
                onClicked: encoder.getStatus(0x05)

            }

            RoundButton {
                id: corpoSButton
                y: 300
                text: ""
                anchors.left: ditoSButton.right
                anchors.leftMargin: 60
                antialiasing: true
                hoverEnabled: false
                enabled: false
                //palette.button: "yellow"
                onClicked: encoder.getStatus(0x01)

            }

            RoundButton {
                id: manineSButton
                y: 300
                text: ""
                anchors.left: corpoSButton.right
                anchors.leftMargin: 60
                antialiasing: true
                hoverEnabled: false
                enabled: false
                //palette.button: "yellow"
                onClicked: encoder.getStatus(0x02)

            }

            RoundButton {
                id: chiusuraSButton
                y: 300
                text: ""
                anchors.left: manineSButton.right
                anchors.leftMargin: 60
                antialiasing: true
                hoverEnabled: false
                enabled: false
                //palette.button: "yellow"
                onClicked: encoder.getStatus(0x03)

            }

            Label {
                id: label1
                x: 415
                y: 350
                width: 90
                text: qsTr("LUNETTA")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 13
            }

            Label {
                id: label2
                x: 515
                y: 350
                width: 90
                text: qsTr("NASTRO")
                font.pointSize: 13
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: label3
                x: 115
                y: 350
                width: 90
                text: qsTr("CORPO")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 13
            }

            Label {
                id: label4
                x: 215
                y: 350
                width: 90
                text: qsTr("MANINE")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 13
            }

            Label {
                id: label5
                x: 315
                y: 350
                width: 90
                text: qsTr("CHIUSURA")
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 13
            }

            Button {
                id: manualButton
                x: 648
                y: 100
                text: qsTr("MANUALE")
                anchors.right: parent.right
                anchors.rightMargin: 20
                onClicked: {
                    manualPane.visible = true
                    manualPane.z = 1
                    //encoder.startPosTimer()
                }
            }

            Button {
                id: fasiButton
                x: 648
                y: 170
                text: qsTr("FASI")
                anchors.right: parent.right
                anchors.rightMargin: 20

            }

            Button {
                id: getStatusButton
                x: 28
                y: 320
                text: qsTr("GET STATUS")
                anchors.right: parent.right
                anchors.rightMargin: 20
                onClicked: encoder.getStatus(0xAB)
            }

            Button {
                id: paramButton
                x: 656
                y: 240
                text: qsTr("PARAMETRI")
                anchors.right: parent.right
                anchors.rightMargin: 20
            }

            TextArea{
                id: textArea
                x: 20
                y: 92
                width: 594
                height: 161
                text: qsTr("Text")
                font.pixelSize: 16
                ScrollBar.vertical: ScrollBar{}
            }

            Label {
                id: mainDitoPoslabel
                x: 15
                y: 390
                width: 90
                text: qsTr("DITO")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                id: mainCorpoPoslabel
                x: 115
                y: 390
                width: 90
                text: qsTr("DITO")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                id: mainManPoslabel
                x: 215
                y: 390
                width: 90
                text: qsTr("DITO")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                id: mainChiuPoslabel
                x: 315
                y: 390
                width: 90
                text: qsTr("DITO")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                id: mainLunePoslabel
                x: 415
                y: 390
                width: 90
                text: qsTr("DITO")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Label {
                id: mainNasPoslabel
                x: 515
                y: 390
                width: 90
                text: qsTr("DITO")
                font.pointSize: 13
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Pane {
            property bool firstRun: false
            id: ditoConfigPane
            z: 0
            anchors.fill: parent
            visible: false
            Component.onCompleted: firstRun = true
            onVisibleChanged: {
                if(firstRun) {
                    visible ? encoder.configParameter(0x00, 0x16, 0x01, 0x00) : null
                    console.log("First run")
                }
            }

            Label {
                id: ditoConfigPanelabel
                x: 303
                y: 0
                width: 350
                height: 30
                text: qsTr("PARAMETRI MOTORE DITO")
                wrapMode: Text.NoWrap
                font.pointSize: 20
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Label {
                id: label6
                x: 20
                y: 45
                height: 20
                text: qsTr("STEP MODE")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            ComboBox {
                id: stepCombo
                x: 173
                y: 45
                height: 35
                model: stepComboModel
                textRole: "display"
                onCurrentIndexChanged: {
                    if(ditoConfigPane.visible && !ditoConfigPane.firstRun) {
                    console.log("Config step mode " + currentIndex)
                    encoder.configParameter(0x00, 0x16, 0x00, currentIndex)
                    }
                    ditoConfigPane.firstRun = false
                }

            }

            Label {
                id: label7
                x: 20
                y: 80
                height: 20
                text: qsTr("OVERCURRENT TH")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            ComboBox {
                id: ocCombo
                x: 173
                y: 80
                height: 35
                visible: true
                model: ocdTHComboModel
                textRole: "display"
            }

            Button {
                id: ditoConfigCloseButton
                x: 653
                y: 395
                text: qsTr("SALVA")
                onClicked: {
                    ditoConfigPane.visible = false
                    ditoConfigPane.z = 0
                }
            }

        }

        Pane {
            id: phasesConfigPane
            z: -1
            anchors.fill: parent
            visible: false
        }

        Pane {
            id: manualPane
            z: -1
            anchors.fill: parent
            visible: false
            onVisibleChanged: {
                if(visible)
                    encoder.getPosition(0xAB)
            }

            TextField {
                id: encoderText2
                x: 300
                y: 10
                text: qsTr("Text Field")
                font.pointSize: 16
                horizontalAlignment: Text.AlignHCenter
            }

            Row {
                id: row
                x: 50
                y: 55
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
                    checkable: true
                    highlighted: false
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 320
                    onCheckedChanged: {
                        checked ? encoder.goToManual(0x04) : encoder.goToManual(0x00)
                        encoder.getPosition(0xAB);
                    }
                }

                Label {
                    id: ditoPosLabel
                    width: 150
                    height: 30
                    text: qsTr("Posizione:")
                    anchors.left: ditoPos1Button.right
                    anchors.leftMargin: 160
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
            }

            Row {
                id: row1
                x: 50
                y: 105
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
                    enabled: ditoPos1Button.checked
                    checkable: true
                    anchors.verticalCenter: parent.verticalCenter
                    transformOrigin: Item.Center
                    anchors.rightMargin: 320
                    anchors.right: parent.right
                    onCheckedChanged:  {
                        checked ? encoder.goToManual(0x07) : encoder.goToManual(0x02)
                        checked ? maninePos1Button.checked = true : maninePos1Button.checked = false
                    }
                }

                Label {
                    id: corpoPosLabel
                    width: 150
                    height: 30
                    text: qsTr("Posizione:")
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: corpoPos1Button.right
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.leftMargin: 160
                }
                spacing: 2
            }

            Row {
                id: row2
                x: 50
                y: 165
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
                    checkable: true
                    enabled: false
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 320
                    transformOrigin: Item.Center
                    anchors.right: parent.right
                    onCheckedChanged:  {
                        checked ? encoder.goToManual(0x06) : encoder.goToManual(0x1)
                        encoder.getPosition(0xAB);
                    }
                }

                Button {
                    id: maninePos2Button
                    y: 2
                    width: 80
                    height: 30
                    text: qsTr("POS 2")
                    enabled: maninePos1Button.checked
                    checkable: true
                    anchors.left: maninePos1Button.right
                    anchors.leftMargin: 40
                    anchors.verticalCenter: parent.verticalCenter
                    transformOrigin: Item.Center
                    onCheckedChanged:  {
                        checked ? encoder.goToManual(11) : encoder.goToManual(14)
                        encoder.getPosition(0xAB);
                    }
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
                y: 225
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
                    enabled: maninePos2Button.checked
                    checkable: true
                    anchors.verticalCenter: parent.verticalCenter
                    transformOrigin: Item.Center
                    anchors.rightMargin: 320
                    anchors.right: parent.right
                    onCheckedChanged:  {
                        checked ? encoder.goToManual(10) : encoder.goToManual(13)
                        encoder.getPosition(0xAB);
                    }
                }

                Button {
                    id: chiusuraPos2Button
                    y: 2
                    width: 80
                    height: 30
                    text: qsTr("POS 2")
                    enabled: chiusuraPos1Button.checked
                    checkable: true
                    anchors.left: chiusuraPos1Button.right
                    anchors.leftMargin: 40
                    anchors.verticalCenter: parent.verticalCenter
                    transformOrigin: Item.Center
                    onCheckedChanged:  {
                        checked ? encoder.goToManual(12) : encoder.goToManual(15)
                        encoder.getPosition(0xAB);
                    }
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
                y: 285
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
                    enabled: false
                    checkable: true
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
                    enabled: false
                    checkable: true
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
                y: 345
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
                    id: nastroPos1Button
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
                    id: nastroPos2Button
                    y: 2
                    width: 80
                    height: 30
                    text: qsTr("AVANZ")
                    anchors.left: nastroPos1Button.right
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
                    anchors.left: nastroPos2Button.right
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    anchors.leftMargin: 40
                }
                spacing: 2
            }

            Button {
                id: quitManualPane
                x: 650
                y: 417
                height: 42
                text: qsTr("CHIUDI")
                onClicked: {
                    manualPane.z = 0
                    manualPane.visible = false;
                }
            }
        }
    }
}
