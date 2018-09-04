#ifndef MOTORS_H
#define MOTORS_H

#include <QObject>

class Motors : public QObject
{
    Q_OBJECT

public:
    Motors(int _id = 0);

    int getMotorId();
    void setMotorStatus(int _status);
    int getMotorStatus();
    void setMotorConfig(int _config);
    int getMotorConfig();

    int id;
    int status;
    int config;
    QString name;
    int startPos1;
    int startPos2;
    int pos1;
    int pos2;
    int speed;

public slots:

signals:

private:

};

#endif // MOTORS_H
