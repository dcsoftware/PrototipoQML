#ifndef ENCTHREAD_H
#define ENCTHREAD_H

#include <QObject>
#include <QThread>

class EncThread : public QThread
{
    Q_OBJECT

public:
    EncThread(QObject *parent = 0);
    ~EncThread();

    static void cbf(int _deg);

protected:
    void run() override;
    int exec();

signals:
    void encupdate(const QString text);

private:
    static void* enc;
};

#endif // ENCTHREAD_H
