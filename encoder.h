#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>
#include <QTimer>
#include <pigpiocommunication.h>
#include <values.h>

class Encoder : public QObject
{
    Q_OBJECT

    QTimer *encTimer, *posTimer;

public:
    Encoder();
    void setSerialPort();
    void decodeData(QByteArray _data);


public slots:
    void getPhasesData();
    void encTimerSlot();
    void posTimerSlot();
    void startTimer();
    void startPosTimer();
    void stopTimer();
    void stopPosTimer();
    void resetTimer();
    void serialDataReady();
    void setResetMotor(int _motor);
    void goToManual(int _posId);
    void getStatus(int _motor);
    void setHomePos(int _motor);
    void getConfig(int _motor);
    void getPosition(int _motor);
    void setParam(int _motor, int _param);
    void getParam(int _motor, int _param);
    void setSoftStop(int _motor);
    void moveCommand(int _motor, long _pos, int _dir);
    void goToCommand(int _motor, long _pos, int _dir);
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
    void posUpdated(int _motor, QString _pos);
    void stepModeUpdated(int _motor, int _data);
    void ocdThUpdated(int _motor, int _data);
    void ocdSDUpdated(int _motor, int _data);
    void oscUpdated(int _motor, int _data);
    void maxSpeedUpdated(int _motor, long _data);
    void fullSpeedUpdated(int _motor, long _data);
    void accUpdated(int _motor, long _data);
    void decUpdated(int _motor, long _data);


private:
    QSerialPort *serialPort = nullptr;

};

#endif // ENCODER_H
