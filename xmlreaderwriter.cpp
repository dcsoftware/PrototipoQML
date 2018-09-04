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
    QFile file(":/xml/motors.xml");


    // Sostituire con QFile::exists() e poi fare la copia e poi istanziare il device senza creare 2 QFile
    if(file.exists()) {
        if(QFile::exists("/home/pi/motorsnew.xml")) {
            bool rem = QFile::remove("/home/pi/motorsnew.xml");
            qDebug() << "Deleting old File is " << rem;
        }
        bool ret = file.copy("/home/pi/motorsnew.xml");
        bool ret1 = QFile::setPermissions("/home/pi/motorsnew.xml", QFile::WriteOwner | QFile::ReadOwner);

        qDebug() << "copy is: " << ret << " and set permission is " << ret1;
    }

    QFile newFile("/home/pi/motorsnew.xml");

    if (!newFile.open(QFile::ReadWrite) || !doc.setContent(&newFile)) {
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
