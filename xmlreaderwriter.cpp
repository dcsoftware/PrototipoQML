#include "xmlreaderwriter.h"
#include <QDomDocument>
#include <QFile>
#include <QtDebug>
#include <QXmlStreamReader>
#include <motors.h>

static QDomDocument doc;


XmlReaderWriter::XmlReaderWriter()
{
    QString errMsg;
    QFileDevice::FileError err = QFileDevice::NoError;
    //QFile file(":/xml/motors.xml");


    if(QFile::exists(":/xml/motors.xml")) {
        if(QFile::exists("/home/pi/motorsnew.xml")) {
            bool rem = QFile::remove("/home/pi/motorsnew.xml");
            qDebug() << "Deleting old File is " << rem;
        }
        bool ret = QFile::copy(":/xml/motors.xml", "/home/pi/motorsnew.xml");
        bool ret1 = QFile::setPermissions("/home/pi/motorsnew.xml", QFile::WriteOwner | QFile::ReadOwner);

        qDebug() << "copy is: " << ret << " and set permission is " << ret1;
    }

    QFile file("/home/pi/motorsnew.xml");

    if (!file.open(QFile::ReadWrite) || !doc.setContent(&file)) {
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
        QDomNode n = motors.item(i);
        QDomElement id = n.firstChildElement("id");
        QDomElement name = n.firstChildElement("name");

    }
}

void XmlReaderWriter::writeDoc()
{

}
