#ifndef PIGPIOCOMMUNICATION_H
#define PIGPIOCOMMUNICATION_H

#include <QObject>
#include <QSerialPort>

class PigpioCommunication : public QObject
{
    Q_OBJECT

public:
    PigpioCommunication();
    void setSerialPort();
    void dataReady(QByteArray _data);

public slots:
    void serialDataReady1();
    void getStatus(int _motor);
    void getConfig(int _motor);
    void getPosition(int _motor);
    void setParam(int _motor, int _param);
    void getParam(int _motor, int _param);
    void setSoftSTop(int _motor);
    void move(int _motor, int _pos, int _speed);

signals:
    void readSerialData();
    void statusUpdated(int _motor, bool _status);
    void configUpdated(int _motor, int _config);
    void posUpdated(int _motor, long _pos);

private:
    QSerialPort *serialPort = nullptr;
};

#endif // PIGPIOCOMMUNICATION_H
