#ifndef I2CCOM_H
#define I2CCOM_H

#include <QObject>

class I2CCom : public QObject
{
    Q_OBJECT

public:
    I2CCom();
    int speed;
    int steps;
    void setupI2C();

public slots:
    void getStatus(int _motor);
    void getAllStatus();
    void getPosition(int _motor);
    void getAllPosition();
    void run(int _motor, int _speed, int _dir);
    void move(int _motor, int _steps, int _dir);

signals:
    void updatePosition(int motor, int position);
    void updateStatus(int motor, bool status);
};

#endif // I2CCOM_H
