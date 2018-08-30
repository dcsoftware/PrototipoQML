#include "xmlreaderwriter.h"
#include <QDomDocument>
#include <QFile>
#include <QtDebug>

static QDomDocument doc;


XmlReaderWriter::XmlReaderWriter()
{
    QString errMsg;
    QFileDevice::FileError err = QFileDevice::NoError;
    //file = new QFile(":/xml/motors.xml");
    QFile file(":/xml/motors.xml");

    if (!file.open(QFile::ReadOnly) || !doc.setContent(&file)) {
        errMsg = file.errorString();
        err = file.error();
           qDebug() << "Failed to open file";
           qDebug() << errMsg;
    }
    //readDoc();
}

void XmlReaderWriter::readDoc()
{
    QDomNodeList motors = doc.elementsByTagName("motor");
    for(int i = 0; i < motors.size(); i++) {

    }
}

void XmlReaderWriter::writeDoc()
{

}
