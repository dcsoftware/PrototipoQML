#ifndef XMLREADERTHREAD_H
#define XMLREADERTHREAD_H

#include <QThread>


class XmlReaderThread : public QThread
{
    Q_OBJECT

public:
    XmlReaderThread(QObject *parent = 0);
    ~XmlReaderThread();

public slots:

signals:
    void resultReady(const int _index, const QStringList pos, const QStringList enc, const QStringList mot, const QStringList steps, const QStringList dir);
protected:
    void run() override;

};

#endif // XMLREADERTHREAD_H
