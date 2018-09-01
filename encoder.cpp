#include "encoder.h"
#include <QDebug>
#include <powerstepregisters.h>
#include <QThread>
#include <pigpiocommunication.h>
#include <motors.h>
#include <QtSerialPort>
#include <QSerialPortInfo>


#define SLAVE_ADDRESS 0x04

static int degrees, attempt;
static QByteArray dataOut, dataIn;
static bool answer = false, firstStatus = false;

Encoder::Encoder() : serialPort(new QSerialPort(this))
{
    encTimer = new QTimer();

    connect(encTimer, SIGNAL(timeout()), this, SLOT(encTimerSlot()));
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialDataReady()));

    setSerialPort();

}

void Encoder::setSerialPort() {
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

void Encoder::serialDataReady()
{
    dataIn.append(serialPort->readAll());

    int l = dataIn.size();

    if(static_cast<int>(dataIn[l -1]) == ARDU_STOP) {
        qDebug() << "Reception Complete ";
        for (int i = 0; i < dataIn.size(); i++) {
            QString asHex = QString("%1").arg(static_cast<int>(dataIn[i]), 0, 16);
            qDebug() << asHex;
        }

        decodeData(dataIn);
        dataIn.clear();
    }
}

void Encoder::decodeData(QByteArray _data)
{
    int motor = _data[MOTOR_NUM];

    if(answer) {
        if(dataIn[COMMAND] == dataOut[COMMAND]) {
            qDebug() << "Command sent and received OK";
            answer = false;
            dataOut.clear();
        }
        else {
            qDebug() << "Command sent and received NOT OK";
            answer = false;
            dataOut.clear();
        }
    }

        switch(_data[COMMAND]){
        case GET_STATUS:{
            if(motor == ALL_MOTORS){
                qDebug() << "All motor status, length: " << _data.size();
                for(int i = 0; i < NUM_BOARDS; i++) {
                    int _m = _data[i*3+3];
                    int status = (_data[i*3+4] << 8) | _data[i*3+5];
                    qDebug() << "Motor " << _m << " status: " << status;
                    emit statusUpdated(_data[i*3+3], (status == 0x00) ? false : true);
                }
            } else {
                int status = (_data[MOTOR_NUM + 1] << 8) | _data[MOTOR_NUM + 2];
                qDebug() << "Motor " << motor << " status: " << status;
                emit statusUpdated(motor, (status == 0x00) ? false : true);
            }
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
            emit posUpdated(motor, pos);
            break;
        }
        }
}

void Encoder::getStatus(int _motor)
{
    attempt = 0;
    while(!serialPort->isOpen()){
        setSerialPort();
        if(attempt == 4)
            break;
        attempt++;
    }

    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = GET_STATUS;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
    answer = true;
}

void Encoder::getConfig(int _motor)
{
    attempt = 0;
    while(!serialPort->isOpen()){
        setSerialPort();
        if(attempt == 4)
            break;
        attempt++;
    }
    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = CONFIG;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
    answer = true;
}

void Encoder::getPosition(int _motor)
{
    attempt = 0;
    while(!serialPort->isOpen()){
        setSerialPort();
        if(attempt == 4)
            break;
        attempt++;
    }
    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = ABS_POS;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
}

void Encoder::getParam(int _motor, int _param)
{
    attempt = 0;
    while(!serialPort->isOpen()){
        setSerialPort();
        if(attempt == 4)
            break;
        attempt++;
    }

    dataOut.resize(5);

    dataOut[0] = RPI_START;
    dataOut[1] = GET_PARAM;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = static_cast<char>(_param);
    dataOut[4] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
}

void Encoder::setParam(int _motor, int _param)
{
    attempt = 0;
    while(!serialPort->isOpen()){
        setSerialPort();
        if(attempt == 4)
            break;
        attempt++;
    }

    dataOut.resize(5);

    dataOut[0] = RPI_START;
    dataOut[1] = SET_PARAM;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = static_cast<char>(_param);
    dataOut[4] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
}

void Encoder::moveMotor(int _motor, unsigned long _pos, int _dir)
{
    attempt = 0;
    while(!serialPort->isOpen()){
        setSerialPort();
        if(attempt == 4)
            break;
        attempt++;
    }

    qDebug() << "Received MOVE command";

    dataOut.resize(8);

    dataOut[0] = RPI_START;
    dataOut[1] = MOVE;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = static_cast<char>((_pos >> 16));
    dataOut[4] = static_cast<char>((_pos >> 8));
    dataOut[5] = static_cast<char>(_pos);
    dataOut[6] = static_cast<char>(_dir);
    dataOut[7] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
}

void Encoder::setSoftSTop(int _motor)
{
    attempt = 0;
    while(!serialPort->isOpen()){
        setSerialPort();
        if(attempt == 4)
            break;
        attempt++;
    }

    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = SOFT_STOP;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
}

void Encoder::configParameter(int _motor, int _param, int _getSet, int _data)
{
    attempt = 0;
    while(!serialPort->isOpen()){
        setSerialPort();
        if(attempt == 4)
            break;
        attempt++;
    }

    dataOut.resize(6);

    dataOut[0] = RPI_START;
    dataOut[1] = static_cast<char>(_param);
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = static_cast<char>(_getSet);
    dataOut[4] = static_cast<char>(_data);
    dataOut[5] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
    answer = true;
}

void Encoder::configSpeed(int _motor, int _param, int _getSet, unsigned long _data)
{
    attempt = 0;
    while(!serialPort->isOpen()){
        setSerialPort();
        if(attempt == 4)
            break;
        attempt++;
    }

    dataOut.resize(8);

    dataOut[0] = RPI_START;
    dataOut[1] = static_cast<char>(_param);
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = static_cast<char>(_getSet);
    dataOut[4] = static_cast<char>((_data >> 16));
    dataOut[5] = static_cast<char>((_data >> 8));
    dataOut[6] = static_cast<char>(_data);
    dataOut[7] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
    answer = true;
}

void Encoder::configFrequency(int _motor, int _param, int _getSet, int _mul, int _div)
{
    attempt = 0;
    while(!serialPort->isOpen()){
        setSerialPort();
        if(attempt == 4)
            break;
        attempt++;
    }

    dataOut.resize(7);

    dataOut[0] = RPI_START;
    dataOut[1] = static_cast<char>(_param);
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = static_cast<char>(_getSet);
    dataOut[4] = static_cast<char>(_mul);
    dataOut[5] = static_cast<char>(_div);
    dataOut[6] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(50);
    serialPort->waitForReadyRead(50);
    answer = true;
}

void Encoder::startTimer()
{
    encTimer->start(50);
    qDebug() << "Starting encoder";
}

void Encoder::stopTimer()
{
    encTimer->stop();
    qDebug() << "Stopping encoder";
}

void Encoder::encTimerSlot()
{
    if(degrees < 360){
        degrees++;
    } else {
        resetTimer();
    }

    emit updateEncoder(degrees);
    qDebug() << "Degrees: " << degrees;
    switch(degrees)
    {
    case 20:
        qDebug() << "Degrees: 100 - DITO AVANTI";

        moveMotor(0x00, 1000, 1);
        //wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, DITO);
    break;
    case 50:
        qDebug() << "Degrees: 200 - DITO BACK";
        moveMotor(0x00, 1000, 0);
        //wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, LUNETTA);
        break;
    case 100:
        qDebug() << "Encoder: 300 - NASTRO MOVE";
        moveMotor(0x01, 500, 1);
        //wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, NASTRO);
    break;
    case 130:
        qDebug() << "Encoder: 300 - NASTRO MOVE";
        moveMotor(0x01, 500, 0);
        //wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, NASTRO);
    break;
    case 180:
        qDebug() << "Encoder: 300 - NASTRO MOVE";
        moveMotor(0x02, 1000, 1);
        //wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, NASTRO);
    break;
    case 210:
        qDebug() << "Encoder: 300 - NASTRO MOVE";
        moveMotor(0x02, 1000, 0);
        //wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, NASTRO);
    break;
    default:
        qDebug() << "Default action encoder switch statement;";
        break;
    }
}

void Encoder::resetTimer()
{
    degrees = 0;
    emit updateEncoder(degrees);
}


