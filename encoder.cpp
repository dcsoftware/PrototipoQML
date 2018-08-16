#include "encoder.h"
#include <QDebug>
#include <wiringPiI2C.h>
#include <powerstepregisters.h>
#include <i2ccommands.h>
#include <QThread>

#define SLAVE_ADDRESS 0x04

static int degrees;
static int i2c_slave;

Encoder::Encoder()
{
    encTimer = new QTimer();

    connect(encTimer, SIGNAL(timeout()), this, SLOT(encTimerSlot()));

    setupI2C();
}

void Encoder::setupI2C()
{
    i2c_slave = wiringPiI2CSetup(SLAVE_ADDRESS);
}

void Encoder::getMotorStatus()
{
    qDebug() << "Setting Active motor - DITO";

    wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, DITO);
    QThread::msleep(100);
    int ditoStatus = wiringPiI2CReadReg8(i2c_slave, GET_STATUS);
    QThread::msleep(100);

    qDebug() << "DITO Status: " << ditoStatus;
    qDebug() << "Setting Active motor  - CORPO";

    wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, CORPO);
    QThread::msleep(100);
    int corpoStatus = wiringPiI2CReadReg8(i2c_slave, GET_STATUS);
    QThread::msleep(100);

    qDebug() << "CORPO Status: " << corpoStatus;
    qDebug() << "Setting Active motor  - MANINE";

    wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, MANINE);
    QThread::msleep(100);
    int manineStatus = wiringPiI2CReadReg8(i2c_slave, GET_STATUS);
    QThread::msleep(100);

    qDebug() << "MANINE Status: " << manineStatus;
    qDebug() << "Setting Active motor  - CHIUSURA";

    wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, CHIUSURA);
    QThread::msleep(100);
    int chiusuraStatus = wiringPiI2CReadReg8(i2c_slave, GET_STATUS);
    QThread::msleep(100);

    qDebug() << "CHIUSURA Status: " << chiusuraStatus;
    qDebug() << "Setting Active motor  - LUNETTA";

    wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, LUNETTA);
    QThread::msleep(100);
    int lunettaStatus = wiringPiI2CReadReg8(i2c_slave, GET_STATUS);
    QThread::msleep(100);

    qDebug() << "LUNETTA Status: " << lunettaStatus;
    qDebug() << "Setting Active motor  - NASTRO";

    wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, NASTRO);
    QThread::msleep(100);
    int nastroStatus = wiringPiI2CReadReg8(i2c_slave, GET_STATUS);
    QThread::msleep(100);

    qDebug() << "NASTRO Status: " << nastroStatus;

    emit closePopup();
}

void Encoder::startTimer()
{
    encTimer->start(100);
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
    case 100:
        qDebug() << "Degrees: 100 - DITO MOVE";

        wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, DITO);
        QThread::msleep(5);
    break;
    case 200:
        qDebug() << "Degrees: 200 - LUNETTA MOVE";

        wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, LUNETTA);
        QThread::msleep(5);
        break;
    case 300:
        qDebug() << "Encoder: 300 - NASTRO MOVE";

        wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, NASTRO);
        QThread::msleep(5);
    break;
    default:
        qDebug() << "Default action encoder switch statement;";
        break;
    }
}

void Encoder::resetTimer()
{
    degrees = 0;
}


