#include "xmlreaderwriter.h"
#include <QDomDocument>
#include <QFile>
#include <QtDebug>


XmlReaderWriter::XmlReaderWriter()
{
    file = new QFile("/xml/motors.xml");

    if (!file->open(QIODevice::ReadWrite) || !doc->setContent(file)) {
           qDebug() << "Failed to open file";
    }

}

void XmlReaderWriter::readDoc()
{
    QDomNodeList motors = doc->elementsByTagName("motor");
    for(int i = 0; i < motors.size(); i++) {

    }
}

void XmlReaderWriter::writeDoc()
{

}
