#include "i2ccom.h"
#include <wiringPiI2C.h>
#include <powerstepregisters.h>
#include <QDebug>
#include <QThread>

#define SLAVE_ADDRESS 0x04

static int i2c_slave;


I2CCom::I2CCom()
{
    //setupI2C();
}

void I2CCom::setupI2C()
{
    i2c_slave = wiringPiI2CSetup(SLAVE_ADDRESS);
}

void I2CCom::getStatus(int _motor)
{
    //wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, _motor);
    QThread::msleep(500);
    int status = wiringPiI2CReadReg16(i2c_slave, GET_STATUS);
    qDebug() << "Status motor " << _motor << " : " << status;
    emit updateStatus(_motor, (status != 0x00)?true:false);
}

void I2CCom::getAllStatus()
{
    for(int i = 0; i < 6; i++) {
        //wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, i);
        QThread::msleep(300);
        int status = wiringPiI2CReadReg16(i2c_slave, GET_STATUS);
        QThread::msleep(500);
        qDebug() << "Status motor " << i << " : " << status;
        emit updateStatus(i, (status != 0x00)?true:false);
    }
}

void I2CCom::getPosition(int _motor)
{
    //wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, _motor);
    QThread::msleep(100);
    int pos = wiringPiI2CReadReg16(i2c_slave, ABS_POS);
    emit updatePosition(_motor, pos);
}

void I2CCom::getAllPosition()
{
    for(int i = 0; i < 6; i++){
        //wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, i);
        QThread::msleep(100);
        int pos = wiringPiI2CReadReg16(i2c_slave, ABS_POS);
        QThread::msleep(100);
        emit updatePosition(i, pos);
    }
}

void I2CCom::run(int _motor, int _speed, int _dir)
{
    //wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, _motor);
    QThread::msleep(100);
    speed = _speed;
    wiringPiI2CWriteReg16(i2c_slave, RUN, _speed);
}

void I2CCom::move(int _motor, int _steps, int _dir)
{
    //wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, _motor);
    QThread::msleep(100);
    steps = _steps;
    wiringPiI2CWriteReg16(i2c_slave, MOVE, _steps);
}
