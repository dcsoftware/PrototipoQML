#ifndef XMLREADERWRITER_H
#define XMLREADERWRITER_H

#include <QObject>
#include <QFile>
#include <QDomDocument>

class XmlReaderWriter : public QObject
{
    Q_OBJECT

    //QFile *file;
    //QDomDocument *doc;

public:
    XmlReaderWriter();

    void readDoc();
    void writeDoc();

public slots:

signals:


};

#endif // XMLREADERWRITER_H
