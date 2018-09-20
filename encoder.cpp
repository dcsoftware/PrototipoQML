#include "encoder.h"
#include <QDebug>
#include <powerstepregisters.h>
#include <QThread>
#include <pigpiocommunication.h>
#include <motors.h>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <values.h>
#include <xmlreaderwriter.h>
#include <debugutils.h>


#define SLAVE_ADDRESS 0x04
#define WAIT_DATA_READ 30
#define WAIT_DATA_WRITE 30

static int degrees, attempt;
static QByteArray dataOut, dataIn;
static bool answer = false;
static int dataBuffer;
static long longBuffer;

static QStringList posId, encoders, motors, steps, dir;
static int p[14], e[14], m[14], d[14];
static unsigned long s[14];

Encoder::Encoder() : serialPort(new QSerialPort(this))
{
    qDebug() << "Encoder run";
    encTimer = new QTimer();


    connect(encTimer, SIGNAL(timeout()), this, SLOT(encTimerSlot()));
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialDataReady()));

    setSerialPort();

    getPhasesData();
}

void Encoder::getPhasesData()
{
    posId = XmlReaderWriter::getPositionArray();
    encoders = XmlReaderWriter::getEncoderArray();
    motors = XmlReaderWriter::getMotorsArray();
    steps = XmlReaderWriter::getStepsArray();
    dir = XmlReaderWriter::getDirArray();

    for(int i = 0; i < 14; i++) {
        p[i] = posId.at(i).toInt();
        e[i] = encoders.at(i).toInt();
        m[i] = motors.at(i).toInt();
        s[i] = steps.at(i).toULong();
        d[i] = dir.at(i).toInt();
    }
}

void Encoder::setSerialPort() {
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        if(!info.portName().compare("ttyS0")) {
            qDebug() << "Setting serial port";
            serialPort->setPort(info);
            serialPort->setBaudRate(QSerialPort::Baud57600);
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
        /*for (int i = 0; i < dataIn.size(); i++) {
            QString asHex = QString("%1").arg(static_cast<int>(dataIn[i]), 0, 16);
            qDebug() << asHex;
        }*/

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

void Encoder::setResetMotor(int _motor)
{

    getStatus(_motor);
    QThread::msleep(100);
    setSoftStop(_motor);
    QThread::msleep(100);
    setHomePos(_motor);
}

void Encoder::getStatus(int _motor)
{
    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = GET_STATUS;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(WAIT_DATA_WRITE);
    serialPort->waitForReadyRead(WAIT_DATA_READ);
}

void Encoder::setHomePos(int _motor)
{
    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = RESET_POS;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(WAIT_DATA_WRITE);
    serialPort->waitForReadyRead(WAIT_DATA_READ);
    answer = true;
}

void Encoder::getConfig(int _motor)
{
    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = CONFIG;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(WAIT_DATA_WRITE);
    serialPort->waitForReadyRead(WAIT_DATA_READ);
    answer = true;
}

void Encoder::getPosition(int _motor)
{

    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = ABS_POS;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(WAIT_DATA_WRITE);
    serialPort->waitForReadyRead(WAIT_DATA_READ);
}

void Encoder::getParam(int _motor, int _param)
{
    dataOut.resize(5);

    dataOut[0] = RPI_START;
    dataOut[1] = GET_PARAM;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = static_cast<char>(_param);
    dataOut[4] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(WAIT_DATA_WRITE);
    serialPort->waitForReadyRead(WAIT_DATA_READ);
}

void Encoder::setParam(int _motor, int _param)
{
    dataOut.resize(5);

    dataOut[0] = RPI_START;
    dataOut[1] = SET_PARAM;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = static_cast<char>(_param);
    dataOut[4] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(WAIT_DATA_WRITE);
    serialPort->waitForReadyRead(WAIT_DATA_READ);
}

void Encoder::moveCommand(int _motor, unsigned long _steps, int _dir)
{
    qDebug() << "Received MOVE command";

    dataOut.resize(8);

    dataOut[0] = RPI_START;
    dataOut[1] = MOVE;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = static_cast<char>((_steps >> 16));
    dataOut[4] = static_cast<char>((_steps >> 8));
    dataOut[5] = static_cast<char>(_steps);
    dataOut[6] = static_cast<char>(_dir);
    dataOut[7] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(WAIT_DATA_WRITE);
    serialPort->waitForReadyRead(WAIT_DATA_READ);
}

void Encoder::goToCommand(int _motor, unsigned long _pos, int _dir)
{
    qDebug() << "Received MOVE command";

    dataOut.resize(8);

    dataOut[0] = RPI_START;
    dataOut[1] = GOTO_DIR;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = static_cast<char>((_pos >> 16));
    dataOut[4] = static_cast<char>((_pos >> 8));
    dataOut[5] = static_cast<char>(_pos);
    dataOut[6] = static_cast<char>(_dir);
    dataOut[7] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(WAIT_DATA_WRITE);
    serialPort->waitForReadyRead(WAIT_DATA_READ);
}

void Encoder::setSoftStop(int _motor)
{
    qDebug() << "SOFT STOP";
    dataOut.resize(4);
    dataOut[0] = RPI_START;
    dataOut[1] = SOFT_STOP;
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(WAIT_DATA_WRITE);
    serialPort->waitForReadyRead(WAIT_DATA_READ);
}

void Encoder::configParameter(int _motor, int _param, int _getSet, int _data)
{
    dataOut.resize(6);

    dataOut[0] = RPI_START;
    dataOut[1] = static_cast<char>(_param);
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = static_cast<char>(_getSet);
    dataOut[4] = static_cast<char>(_data);
    dataOut[5] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(WAIT_DATA_WRITE);
    serialPort->waitForReadyRead(WAIT_DATA_READ);
    answer = true;
}

void Encoder::configSpeed(int _motor, int _param, int _getSet, unsigned long _data)
{
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
    serialPort->waitForBytesWritten(WAIT_DATA_WRITE);
    serialPort->waitForReadyRead(WAIT_DATA_READ);
    answer = true;
}

void Encoder::configFrequency(int _motor, int _param, int _getSet, int _mul, int _div)
{
    dataOut.resize(7);

    dataOut[0] = RPI_START;
    dataOut[1] = static_cast<char>(_param);
    dataOut[2] = static_cast<char>(_motor);
    dataOut[3] = static_cast<char>(_getSet);
    dataOut[4] = static_cast<char>(_mul);
    dataOut[5] = static_cast<char>(_div);
    dataOut[6] = RPI_STOP;

    serialPort->write(dataOut);
    serialPort->waitForBytesWritten(WAIT_DATA_WRITE);
    serialPort->waitForReadyRead(WAIT_DATA_READ);
    answer = true;
}

void Encoder::startTimer()
{
    encTimer->start(2);
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
    //qDebug() << "Degrees: " << degrees;

    QString deg = QString::number(degrees);

    for(int i = 0; i < 14; i++){
        if(degrees == e[i])
        {
            qDebug() << "POS ID " << p[i] << "GRADI " << deg << ", MOTORE " << m[i] << ", STEPS " << s[i] << ", DIR " << d[i];
            if(m[i] != 0x05)
                goToCommand(m[i], s[i], d[i]);
            else if(m[i] == 0x05)
                moveCommand(m[i], s[i], d[i]);
        }
    }

    /*if(encoders.contains(deg)) {
        int c = encoders.count(deg);
        if(c > 1) {
            int i1 = encoders.indexOf(deg);
            int i2 = encoders.lastIndexOf(deg);
            moveMotor(motors.at(i2).toInt(), steps.at(i2).toULong(), 0x01);
            qDebug() << "POS ID " << posId.at(i1) << "GRADI " << deg << ", MOTORE " << motors.at(i1) << ", STEPS " << steps.at(i1);
            moveMotor(motors.at(i1).toInt(), steps.at(i1).toULong(), 0x01);
            qDebug() << "POS ID " << posId.at(i2) << "GRADI " << deg << ", MOTORE " << motors.at(i2) << ", STEPS " << steps.at(i2);
        } else if(c == 1) {
            int i0 = encoders.indexOf(deg);
            qDebug() << "POS ID " << posId.at(i0) << "GRADI " << deg << ", MOTORE " << motors.at(i0) << ", STEPS " << steps.at(i0);
            moveMotor(motors.at(i0).toInt(), steps.at(i0).toULong(), 0x01);
        }
    }*/
}

void Encoder::resetTimer()
{
    degrees = 0;
    emit updateEncoder(degrees);
}


