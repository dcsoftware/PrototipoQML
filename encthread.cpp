#include "encthread.h"
#include <red.h>
#include <QDebug>
#include <pigpiod_if2.h>

int optMode = RED_MODE_STEP;
int deg;
bool cb = false, first = false;

EncThread::EncThread(QObject *parent) : QThread(parent)
{
}

void EncThread::cbf(int _deg)
{
    qDebug() << "CALLBACK " << _deg;
    deg = _deg;
    cb = true;
}

void EncThread::run()
{
    if(!first) {

        qDebug() << "FIRST RUN";
        int pi;
        RED_t *renc;

        pi = pigpio_start(NULL, NULL);

        if (pi >= 0)
        {
           qDebug() << "Initializzation OK";
           renc = RED(pi, 21, 16, optMode, cbf);
           RED_set_glitch_filter(renc, 50);

           first = true;

           //if (optSeconds) sleep(optSeconds);
           //else while(1) sleep(60);

           //RED_cancel(renc);

           //pigpio_stop(pi);
        }
    }

    exec();
}

int EncThread::exec() {
    qDebug() << "EXEC RUB";

    while(1) {
        if(cb) {
            qDebug() << "CB calbback insedi run";
            emit encupdate(QString(deg));
            cb = false;
        }
    }
}

EncThread::~EncThread()
{

}
