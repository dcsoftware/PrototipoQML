#include "motors.h"
#include "xmlreaderwriter.h"

Motors::Motors(int _id)
{
    id = _id;

}

int Motors::getMotorId()
{
    return id;
}

void Motors::setMotorStatus(int _status)
{
    status = _status;
}

int Motors::getMotorStatus()
{
    return status;
}

void Motors::setMotorConfig(int _config)
{
    config = _config;
}

int Motors::getMotorConfig()
{
    return config;
}
