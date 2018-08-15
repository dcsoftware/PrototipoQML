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
    QThread::msleep(5);
    int ditoStatus = wiringPiI2CReadReg16(i2c_slave, GET_STATUS);
    QThread::msleep(5);

    qDebug() << "Setting Active motor  - LUNETTA";

    wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, LUNETTA);
    QThread::msleep(5);
    int lunettaStatus = wiringPiI2CReadReg16(i2c_slave, GET_STATUS);
    QThread::msleep(5);

    qDebug() << "Setting Active motor  - NASTRO";

    wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, NASTRO);
    QThread::msleep(5);
    int nastroStatus = wiringPiI2CReadReg16(i2c_slave, GET_STATUS);
    QThread::msleep(5);

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


