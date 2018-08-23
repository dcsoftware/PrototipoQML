#ifndef PIGPIOCOMMUNICATION_H
#define PIGPIOCOMMUNICATION_H

#include <QObject>
#include <QSerialPort>

class PigpioCommunication : public QObject
{
    Q_OBJECT

public:
    PigpioCommunication();
    void setupCom();
    void setSerialPort();

public slots:
    void serialDataReady();
    void getStatus(int _motor);
    void getConfig(int _motor);
    void getPosition(int _motor);
    void setParam(int _motor, int _param);
    void getParam(int _motor, int _param);

signals:
    void readSerialData();

private:
    QSerialPort *serialPort = nullptr;
    QByteArray serialBuffer;
};

#endif // PIGPIOCOMMUNICATION_H
