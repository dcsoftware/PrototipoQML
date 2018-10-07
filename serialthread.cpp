#include "serialthread.h"
#include <QSerialPortInfo>
#include <powerstepregisters.h>

static QByteArray dataOut, dataIn;
static int dataBuffer;
static long longBuffer;


SerialThread::SerialThread(QObject *parent) : QThread(parent), serialPort2(new QSerialPort(this))
{
    restart = false;
    abort = false;

    connect(serialPort2, SIGNAL(readyRead()), this, SLOT(serialDataReady()));

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        //qDebug() << "Name : " << info.portName();
        //qDebug() << "Description : " << info.description();
        //qDebug() << "Manufacturer: " << info.manufacturer();

        if(!info.portName().compare("ttyS0")) {
            qDebug() << "Setting serial port";
            serialPort2->setPort(info);
            serialPort2->setBaudRate(QSerialPort::Baud115200);
            serialPort2->setDataBits(QSerialPort::Data8);
            serialPort2->setParity(QSerialPort::NoParity);
            serialPort2->setStopBits(QSerialPort::OneStop);
            serialPort2->setFlowControl(QSerialPort::NoFlowControl);
            if(serialPort2->open(QIODevice::ReadWrite))
                qDebug() << "Serial port opened";
        }
    }
}

void SerialThread::serialDataReady2()
{
    dataIn.append(serialPort2->readAll());

    int l = dataIn.size();

    if(static_cast<int>(dataIn[l -1]) == ARDU_STOP) {
        //qDebug() << "Reception Complete ";
        /*for (int i = 0; i < dataIn.size(); i++) {
            QString asHex = QString("%1").arg(static_cast<int>(dataIn[i]), 0, 16);
            qDebug() << asHex;
        }*/

        decodeData2(dataIn);
        dataIn.clear();
    }
}

void SerialThread::decodeData2(QByteArray _data)
{
    int motor = _data[MOTOR_NUM];

    switch(_data[COMMAND]){
        case GET_STATUS:{
            if(motor == ALL_MOTORS){
                qDebug() << "All motor status, length: " << _data.size();
                for(int i = 0; i < NUM_BOARDS; i++) {
                    dataBuffer = static_cast<int>((_data[i*3+4] << 8)) | static_cast<int>(_data[i*3+5]);
                    //qDebug() << "Motor " << _data[i*3+3] << " status: " << dataBuffer;
                    //decodeStatus(dataBuffer);
                    //emit statusUpdated(_data[i*3+3], (dataBuffer == 0x00) ? false : true);
                }
            } else {
                dataBuffer = static_cast<int>((_data[MOTOR_NUM + 1] << 8)) | static_cast<int>(_data[MOTOR_NUM + 2]);
                //qDebug() << "Motor " << motor << " status: " << dataBuffer;
                //emit statusUpdated(motor, (dataBuffer == 0x00) ? false : true);
            }
            break;
        }
        case GET_STAT_POS:{
            if(motor == ALL_MOTORS){
                //qDebug() << "All motor STATUS and POSITION, length: " << _data.size();
                for(int i = 0; i < NUM_BOARDS; i++) {
                    dataBuffer = static_cast<int>((_data[i*6+4] << 8)) | static_cast<int>(_data[i*6+5]);
                    longBuffer = static_cast<long>((_data[i*6+6] << 16)) | static_cast<long>((_data[i*6+7] << 8)) | static_cast<long>((_data[i*6+8]));
                    //qDebug() << "Motor " << _data[i*3+3] << " status: " << dataBuffer;
                    //decodeStatus(dataBuffer);
                    //emit statusUpdated(_data[i*6+3], (dataBuffer == 0x00) ? false : true);
                    //emit posUpdated(_data[i*6+3], QString::number(longBuffer));
                }
            }
            break;
        }
        case CONFIG:{
            if(motor == ALL_MOTORS){
                qDebug() << "All motor config, length: " << _data.size();
                for(int i = 0; i < NUM_BOARDS; i++) {
                    dataBuffer = static_cast<int>((_data[i*3+4] << 8)) | static_cast<int>(_data[i*3+5]);
                    qDebug() << "Motor " << _data[i*3+3] << " status: " << dataBuffer;
                    //emit configUpdated(_data[i*3+3], dataBuffer);
                }
            } else {
                dataBuffer = static_cast<int>((_data[MOTOR_NUM + 1] << 8)) | static_cast<int>(_data[MOTOR_NUM + 2]);
                qDebug() << "Motor " << motor << " config: " << dataBuffer;
                //emit configUpdated(motor, dataBuffer);
            }
            break;
        }
        case ABS_POS:{
            if(motor == ALL_MOTORS){
                //qDebug() << "All motor position, length: " << _data.size();
                for(int i = 0; i < NUM_BOARDS; i++) {
                    longBuffer = static_cast<long>((_data[i*4+4] << 16)) | static_cast<long>((_data[i*4+5] << 8)) | static_cast<long>((_data[i*4+6]));
                    //qDebug() << "Motor " << _data[i*4+3] << " position: " << longBuffer;
                    //emit posUpdated(_data[i*4+3], QString::number(longBuffer));
                }
            } else {
                longBuffer = static_cast<long>((_data[MOTOR_NUM + 1] << 16)) | static_cast<long>((_data[MOTOR_NUM + 2] << 8)) | static_cast<long>(_data[MOTOR_NUM + 3]);
                //qDebug() << "Motor " << motor << " position: " << longBuffer;
                //emit posUpdated(motor, QString::number(longBuffer));
            }
            break;
        }
        case STEP_MODE: {
            qDebug() << "Motor STEP MODE, length: " << _data.size();
            dataBuffer = static_cast<int>((_data[4]));

            qDebug() << "Motor " << motor << " : " << dataBuffer;

            //emit stepModeUpdated(motor, dataBuffer);
            break;
        }
        case OCD_TH: {
            qDebug() << "Motor OCD TH, length: " << _data.size();
            dataBuffer = static_cast<int>((_data[4]));
            qDebug() << "Motor " << motor << " : " << dataBuffer;
            //emit ocdThUpdated(motor, dataBuffer);
            break;
        }
        case CONFIG_OC_SD: {
            qDebug() << "Motor OCD SHUTDOWN, length: " << _data.size();
            dataBuffer = static_cast<int>((_data[4]));
            qDebug() << "Motor " << motor << " : " << dataBuffer;
            //emit ocdSDUpdated(motor, dataBuffer);
            break;
        }
        case CONFIG_OSC_SEL: {
            qDebug() << "Motor OSCILLATOR MODE, length: " << _data.size();
            dataBuffer = static_cast<int>((_data[4]));
            qDebug() << "Motor " << motor << " OSCILLATOR: " << dataBuffer;
            //emit oscUpdated(motor, dataBuffer);
            break;
        }
        case MAX_SPEED: {
            qDebug() << "Motor MAX SPEED, length: " << _data.size();
            longBuffer = static_cast<long>((_data[4] << 16)) | static_cast<long>((_data[5] << 8)) | static_cast<long>(_data[6]);
            qDebug() << "Motor " << motor << " MAX SPEED: " << longBuffer;
            //emit maxSpeedUpdated(motor, longBuffer);
            break;
        }
        case FS_SPD: {
            qDebug() << "Motor FULL SPEED, length: " << _data.size();
            longBuffer = static_cast<long>((_data[4] << 16)) | static_cast<long>((_data[5] << 8)) | static_cast<long>(_data[6]);
            qDebug() << "Motor " << motor << " FULL SPEED: " << longBuffer;
            //emit fullSpeedUpdated(motor, longBuffer);
            break;
        }
        case ACC: {
            qDebug() << "Motor ACCELERATION, length: " << _data.size();
            longBuffer = static_cast<long>((_data[4] << 16)) | static_cast<long>((_data[5] << 8)) | static_cast<long>(_data[6]);
            qDebug() << "Motor " << motor << " ACC: " << longBuffer;
            //emit accUpdated(motor, longBuffer);
            break;
        }
        case DECEL: {
            qDebug() << "Motor DECELERATION, length: " << _data.size();
            longBuffer = static_cast<long>((_data[4] << 16)) | static_cast<long>((_data[5] << 8)) | static_cast<long>(_data[6]);
            qDebug() << "Motor " << motor << " DEC: " << longBuffer;
            //emit accUpdated(motor, longBuffer);
            break;
        }
    }
}



SerialThread::~SerialThread()
{
    abort = true;
    wait();
}

void SerialThread::run()
{

}
