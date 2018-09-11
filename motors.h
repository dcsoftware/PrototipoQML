#ifndef MOTORS_H
#define MOTORS_H

#include <QObject>

class Motors : public QObject
{
    Q_OBJECT

public:
    Motors(int _id = 0);
    Motors(int _id, QString _name, const QString _pos[], const QString _degrees[]);

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
    QString degrees[3];
    QString pos[3];


public slots:

signals:

private:

};

#endif // MOTORS_H
