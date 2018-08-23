#include "pigpiocommunication.h"
#include <QDebug>
#include <powerstepregisters.h>
#include <i2ccommands.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <QSerialPort>
#include <QSerialPortInfo>


static int pi, serialHandle;
static QByteArray dataOut, dataIn;
static bool answer = false;


PigpioCommunication::PigpioCommunication() : serialPort(new QSerialPort(this))
{
    //setupCom();

    setSerialPort();

    connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialDataReady()));

}

void PigpioCommunication::setupCom()
{

    pi = wiringPiSetup();

    //pi = pigpio_start(0, 0);
    qDebug() << "Result: " << pi;

    char device[] = "/dev/serial0";

    serialHandle = serialOpen(device, 9600);

    //serialHandle = serial_open(pi, device, 9600, 0);

    qDebug() << "Result: " << serialHandle;

    char buffer[] = "Ciao";
    for (int i = 0; i < 10; i++) {

        serialPutchar(serialHandle, i);
    }
    //int result = serial_write(pi, serialHandle, buffer, 4);

    //qDebug() << "Result: " << result;

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

            serialBuffer.clear();
        }
    }
}

void PigpioCommunication::serialDataReady()
{
    /*serialBuffer.append(serialPort->readAll());
    int serPos;

    while((serPos = serialBuffer.indexOf("\n")) >= 0) {
        serialBuffer = serialBuffer.mid(serPos + 1);
    }*/

    dataIn = serialPort->readAll();

    qDebug() << "Serial Data Ready: ";
    for (int i = 0; i < dataIn.size(); i++) {
        QString asHex = QString("%1").arg(static_cast<int>(dataIn[i]), 0, 16);
        qDebug() << asHex;
    }

    //serialBuffer.clear();
    /*for(int i = 0; i < dataIn.size(); i++)
    {
        qDebug() << dataIn[i] << " ";
    }*/

    //char data[100];

    //int length = serialPort->readLine(data, 20);

    //qDebug() << "Serial read length: " << length << " data: " << data;
}

void PigpioCommunication::getStatus(int _motor)
{
    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = GET_STATUS;
    dataOut[2] = _motor;
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(500);
    serialPort->waitForReadyRead(500);


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

    answer = true;
}

void PigpioCommunication::getPosition(int _motor)
{

}

void PigpioCommunication::getParam(int _motor, int _param)
{

}

void PigpioCommunication::setParam(int _motor, int _param)
{

}
