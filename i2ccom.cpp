#include "i2ccom.h"
#include <wiringPiI2C.h>
#include <powerstepregisters.h>
#include <QDebug>
#include <i2ccommands.h>
#include <QThread>

#define SLAVE_ADDRESS 0x04

static int i2c_slave;


I2CCom::I2CCom()
{
    setupI2C();
}

void I2CCom::setupI2C()
{
    i2c_slave = wiringPiI2CSetup(SLAVE_ADDRESS);
}

int I2CCom::getStatus(int _motor)
{
    wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, _motor);
    QThread::msleep(100);
    return wiringPiI2CReadReg16(i2c_slave, GET_STATUS);
}

void I2CCom::getPosition(int _motor)
{
    wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, _motor);
    QThread::msleep(100);
    int pos = wiringPiI2CReadReg16(i2c_slave, ABS_POS);
    emit updatePosition(_motor, pos);
}

void I2CCom::run(int _motor, int _speed, int _dir)
{
    wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, _motor);
    QThread::msleep(100);
    speed = _speed;
    wiringPiI2CWriteReg16(i2c_slave, RUN, _speed);
}

void I2CCom::move(int _motor, int _steps, int _dir)
{
    wiringPiI2CWriteReg8(i2c_slave, I2C_SET_MOTOR, _motor);
    QThread::msleep(100);
    steps = _steps;
    wiringPiI2CWriteReg16(i2c_slave, MOVE, _steps);
}
