#include "gpiomanager.h"
#include <pigpiod_if2.h>
#include <values.h>

GpioManager::GpioManager(int _pi)
{
    myPi = _pi;
}

void GpioManager::_encCB(uint32_t gpio, uint32_t level)
{
    myEncCallBack;
}

void GpioManager::_encCBEx(int pi, uint32_t gpio, uint32_t level, uint32_t tick, void *user)
{
    GpioManager *mySelf = (GpioManager *) user;
    mySelf->_encCB(gpio, level);
}

void GpioManager::setEncoderCB(encoderCB_t encCallBack)
{
    myPhaseA = gpioEncInput[0];
    myPhaseB = gpioEncInput[1];

    myEncCallBack = encCallBack;

    levA = 0;
    levB = 0;

    for(int i = 0; i < 2; i++){
        set_mode(myPi, myPhaseA, PI_INPUT);
        set_mode(myPi, myPhaseB, PI_INPUT);
        set_pull_up_down(myPi, gpioEncInput[i], PI_PUD_DOWN);
        callback_ex(myPi, gpioEncInput[i], EITHER_EDGE, _encCBEx, this);
    }
}

