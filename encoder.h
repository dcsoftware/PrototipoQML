#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>
#include <QTimer>
#include <pigpiocommunication.h>

class Encoder : public QObject
{
    Q_OBJECT

    QTimer *encTimer;
    //PigpioCommunication *comm;

public:
    Encoder();
    void setSerialPort();
    void decodeData(QByteArray _data);

public slots:
    void encTimerSlot();
    void startTimer();
    void stopTimer();
    void resetTimer();
    void serialDataReady();
    void getStatus(int _motor);
    void getConfig(int _motor);
    void getPosition(int _motor);
    void setParam(int _motor, int _param);
    void getParam(int _motor, int _param);
    void setSoftSTop(int _motor);
    void moveMotor(int _motor, unsigned long _pos, int _dir);
    void configParameter(int _motor, int _param, int _getSet, int data);
    void configSpeed(int _motor, int _param, int _getSet, unsigned long _data);
    void configFrequency(int _motor, int _param, int _getSet, int _mul, int _div);
    //void getMotorStatus();


signals:
    void updateEncoder(int degrees);
    void closePopup();
    void readSerialData();
    void statusUpdated(int _motor, bool _status);
    void configUpdated(int _motor, int _config);
    void posUpdated(int _motor, long _pos);

private:
    QSerialPort *serialPort = nullptr;

};

#endif // ENCODER_H
