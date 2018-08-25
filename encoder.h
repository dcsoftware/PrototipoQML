#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>
#include <QTimer>
#include <pigpiocommunication.h>

class Encoder : public QObject
{
    Q_OBJECT

    QTimer *encTimer;
    PigpioCommunication *comm;

public:
    Encoder();
    void setupI2C();

public slots:
    void encTimerSlot();
    void startTimer();
    void stopTimer();
    void resetTimer();
    //void getMotorStatus();


signals:
    void updateEncoder(int degrees);
    void closePopup();
    void updateMotorStatus(int motor, bool status);
    void moveMotor();
};

#endif // ENCODER_H
