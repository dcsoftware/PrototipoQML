#ifndef XMLREADERWRITER_H
#define XMLREADERWRITER_H

#include <QObject>
#include <QFile>
#include <QDomDocument>
#include <motors.h>

class XmlReaderWriter : public QObject
{
    Q_OBJECT

    //QFile *file;
    //QDomDocument *doc;

public:
    XmlReaderWriter();

    void readElements();
    void updateElement(int _id, int _degrees, unsigned long _ste);
    void updateEncoderPhase(int _id, int _degrees);
    void updateStepPhase(int _id, unsigned long _ste);
    static QStringList getPositionArray();
    static QStringList getEncoderArray();
    static QStringList getMotorsArray();
    static QStringList getStepsArray();
    static QStringList getDirArray();

public slots:

};

#endif // XMLREADERWRITER_H
