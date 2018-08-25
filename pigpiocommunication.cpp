#include "pigpiocommunication.h"
#include <QDebug>
#include <powerstepregisters.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <QSerialPort>
#include <QSerialPortInfo>


static QByteArray dataOut, dataIn;
static bool answer = false;


PigpioCommunication::PigpioCommunication() : serialPort(new QSerialPort(this))
{
    setSerialPort();

    connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialDataReady()));

}

void PigpioCommunication::setSerialPort()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        if(!info.portName().compare("ttyS0")) {
            qDebug() << "Setting serial port";
            serialPort->setPort(info);
            serialPort->setBaudRate(QSerialPort::Baud9600);
            serialPort->setDataBits(QSerialPort::Data8);
            serialPort->setParity(QSerialPort::NoParity);
            serialPort->setStopBits(QSerialPort::OneStop);
            serialPort->setFlowControl(QSerialPort::NoFlowControl);
            if(serialPort->open(QIODevice::ReadWrite))
                qDebug() << "Serial port opened";
        }
    }
}

void PigpioCommunication::serialDataReady()
{
    dataIn.append(serialPort->readAll());

    int l = dataIn.size();

    if(static_cast<int>(dataIn[l -1]) == ARDU_STOP) {
        qDebug() << "Reception Complete ";
        /*for (int i = 0; i < dataIn.size(); i++) {
            QString asHex = QString("%1").arg(static_cast<int>(dataIn[i]), 0, 16);
            qDebug() << asHex;
        }*/

        dataReady(dataIn);
        dataIn.clear();
    }
}

void PigpioCommunication::dataReady(QByteArray _data)
{
    int motor = _data[MOTOR_NUM];

    if(motor == ALL_MOTORS) {
    } else {

        switch(_data[COMMAND]){
        case GET_STATUS:{
            int status = (_data[MOTOR_NUM + 1] << 8) | _data[MOTOR_NUM + 2];
            qDebug() << "Motor " << motor << " status: " << status;
            emit statusUpdated(motor, status);
            break;
        }
        case CONFIG:{
            int config = (_data[MOTOR_NUM + 1] << 8) | _data[MOTOR_NUM + 2];
            qDebug() << "Motor " << motor << " config: " << config;
            emit configUpdated(motor, config);
            break;
        }
        case ABS_POS:{
            long pos = (_data[MOTOR_NUM + 1] << 16) | (_data[MOTOR_NUM + 2] << 8) | _data[MOTOR_NUM + 3];
            qDebug() << "Motor " << motor << " position: " << pos;
            emit configUpdated(motor, pos);
            break;
        }
        }

    }
}

void PigpioCommunication::getStatus(int _motor)
{
    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = GET_STATUS;
    dataOut[2] = _motor;
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
    answer = true;
}

void PigpioCommunication::getConfig(int _motor)
{
    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = CONFIG;
    dataOut[2] = _motor;
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
    answer = true;
}

void PigpioCommunication::getPosition(int _motor)
{
    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = ABS_POS;
    dataOut[2] = _motor;
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
}

void PigpioCommunication::getParam(int _motor, int _param)
{
    dataOut.resize(5);

    dataOut[0] = RPI_START;
    dataOut[1] = GET_PARAM;
    dataOut[2] = _motor;
    dataOut[3] = _param;
    dataOut[4] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
}

void PigpioCommunication::setParam(int _motor, int _param)
{
    dataOut.resize(5);

    dataOut[0] = RPI_START;
    dataOut[1] = SET_PARAM;
    dataOut[2] = _motor;
    dataOut[3] = _param;
    dataOut[4] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
}

void PigpioCommunication::moveMotor(int _motor, int _pos, int _speed)
{
    dataOut.resize(6);

    dataOut[0] = RPI_START;
    dataOut[1] = GOTO;
    dataOut[2] = _motor;
    dataOut[3] = _pos;
    dataOut[4] = _speed;
    dataOut[5] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
}

void PigpioCommunication::setSoftSTop(int _motor)
{
    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = SOFT_STOP;
    dataOut[2] = _motor;
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
}
