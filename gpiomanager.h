#ifndef GPIOMANAGER_H
#define GPIOMANAGER_H

#include <stdint.h>

typedef void (*encoderCB_t)(int);

class GpioManager
{
    int myPi, myPhaseA, myPhaseB, levA, levB, lastPhase;
    encoderCB_t myEncCallBack;

public:
    GpioManager(int _pi);
    void setEncoderCB(encoderCB_t encCallBack);
    void _encCB(uint32_t gpio, uint32_t level);
    static void _encCBEx(int pi, uint32_t gpio, uint32_t level, uint32_t tick, void *user);
};

#endif // GPIOMANAGER_H
