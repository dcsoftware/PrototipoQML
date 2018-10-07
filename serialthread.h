#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H

#include <QObject>
#include <QThread>
#include <QtSerialPort>

class SerialThread : public QThread
{
    Q_OBJECT

public:
    SerialThread(QObject *parent = 0);
    ~SerialThread();

    void sendData();

public slots:
    void serialDataReady2();

signals:

protected:
    void run() override;

private:
    void decodeData2(QByteArray _data);
    bool restart;
    bool abort;
    QSerialPort *serialPort2 = nullptr;
};

#endif // SERIALTHREAD_H
