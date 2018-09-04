#include "encoder.h"
#include <QDebug>
#include <powerstepregisters.h>
#include <QThread>
#include <pigpiocommunication.h>
#include <motors.h>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <values.h>


#define SLAVE_ADDRESS 0x04

static int degrees, attempt;
static QByteArray dataOut, dataIn;
static bool answer = false, firstStatus = false;
static int dataBuffer;
static long longBuffer;

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
                    dataBuffer = static_cast<int>((_data[i*3+4] << 8)) | static_cast<int>(_data[i*3+5]);
                    qDebug() << "Motor " << _data[i*3+3] << " status: " << dataBuffer;
                    emit statusUpdated(_data[i*3+3], (dataBuffer == 0x00) ? false : true);
                }
            } else {
                dataBuffer = static_cast<int>((_data[MOTOR_NUM + 1] << 8)) | static_cast<int>(_data[MOTOR_NUM + 2]);
                qDebug() << "Motor " << motor << " status: " << dataBuffer;
                emit statusUpdated(motor, (dataBuffer == 0x00) ? false : true);
            }
            break;
        }
        case CONFIG:{
            if(motor == ALL_MOTORS){
                qDebug() << "All motor config, length: " << _data.size();
                for(int i = 0; i < NUM_BOARDS; i++) {
                    dataBuffer = static_cast<int>((_data[i*3+4] << 8)) | static_cast<int>(_data[i*3+5]);
                    qDebug() << "Motor " << _data[i*3+3] << " status: " << dataBuffer;
                    emit configUpdated(_data[i*3+3], dataBuffer);
                }
            } else {
                dataBuffer = static_cast<int>((_data[MOTOR_NUM + 1] << 8)) | static_cast<int>(_data[MOTOR_NUM + 2]);
                qDebug() << "Motor " << motor << " config: " << dataBuffer;
                emit configUpdated(motor, dataBuffer);
            }
            break;
        }
        case ABS_POS:{
            if(motor == ALL_MOTORS){
                qDebug() << "All motor position, length: " << _data.size();
                for(int i = 0; i < NUM_BOARDS; i++) {
                    longBuffer = static_cast<long>((_data[i*4+4] << 16)) | static_cast<long>((_data[i*4+5] << 8)) | static_cast<long>((_data[i*4+6]));
                    qDebug() << "Motor " << _data[i*4+3] << " status: " << longBuffer;
                    emit posUpdated(_data[i*4+3], longBuffer);
                }
            } else {
                longBuffer = static_cast<long>((_data[MOTOR_NUM + 1] << 16)) | static_cast<long>((_data[MOTOR_NUM + 2] << 8)) | static_cast<long>(_data[MOTOR_NUM + 3]);
                qDebug() << "Motor " << motor << " position: " << longBuffer;
                emit posUpdated(motor, longBuffer);
            }
            break;
        }
        case STEP_MODE: {
            qDebug() << "Motor STEP MODE, length: " << _data.size();
            dataBuffer = static_cast<int>((_data[4]));
            qDebug() << "Motor " << motor << " : " << dataBuffer;
            emit stepModeUpdated(motor, dataBuffer);
            break;
        }
        case OCD_TH: {
            qDebug() << "Motor OCD TH, length: " << _data.size();
            dataBuffer = static_cast<int>((_data[4]));
            qDebug() << "Motor " << motor << " : " << dataBuffer;
            emit ocdThUpdated(motor, dataBuffer);
            break;
        }
        case CONFIG_OC_SD: {
            qDebug() << "Motor OCD SHUTDOWN, length: " << _data.size();
            dataBuffer = static_cast<int>((_data[4]));
            qDebug() << "Motor " << motor << " : " << dataBuffer;
            emit ocdSDUpdated(motor, dataBuffer);
            break;
        }
        case CONFIG_OSC_SEL: {
            qDebug() << "Motor OSCILLATOR MODE, length: " << _data.size();
            dataBuffer = static_cast<int>((_data[4]));
            qDebug() << "Motor " << motor << " OSCILLATOR: " << dataBuffer;
            emit oscUpdated(motor, dataBuffer);
            break;
        }
        case MAX_SPEED: {
            qDebug() << "Motor MAX SPEED, length: " << _data.size();
            longBuffer = static_cast<long>((_data[4] << 16)) | static_cast<long>((_data[5] << 8)) | static_cast<long>(_data[6]);
            qDebug() << "Motor " << motor << " MAX SPEED: " << longBuffer;
            emit maxSpeedUpdated(motor, longBuffer);
            break;
        }
        case FS_SPD: {
            qDebug() << "Motor FULL SPEED, length: " << _data.size();
            longBuffer = static_cast<long>((_data[4] << 16)) | static_cast<long>((_data[5] << 8)) | static_cast<long>(_data[6]);
            qDebug() << "Motor " << motor << " FULL SPEED: " << longBuffer;
            emit fullSpeedUpdated(motor, longBuffer);
            break;
        }
        case ACC: {
            qDebug() << "Motor ACCELERATION, length: " << _data.size();
            longBuffer = static_cast<long>((_data[4] << 16)) | static_cast<long>((_data[5] << 8)) | static_cast<long>(_data[6]);
            qDebug() << "Motor " << motor << " ACC: " << longBuffer;
            emit accUpdated(motor, longBuffer);
            break;
        }
        case DECEL: {
            qDebug() << "Motor DECELERATION, length: " << _data.size();
            longBuffer = static_cast<long>((_data[4] << 16)) | static_cast<long>((_data[5] << 8)) | static_cast<long>(_data[6]);
            qDebug() << "Motor " << motor << " DEC: " << longBuffer;
            emit accUpdated(motor, longBuffer);
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

void Encoder::setSoftStop(int _motor)
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


