#include "motors.h"
#include "xmlreaderwriter.h"

Motors::Motors(int _id)
{
    id = _id;

}

Motors::Motors(int _id, QString _name, const QString _pos[], const QString _degrees[]) {

    id = _id;
    name = _name;
    for(int i = 0; i < _pos->length(); i++){
        pos[i] = _pos[i];
        degrees[i] = _degrees[i];
    }
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
