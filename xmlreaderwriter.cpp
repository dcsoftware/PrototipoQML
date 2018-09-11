#include "xmlreaderwriter.h"
#include <QDomDocument>
#include <QFile>
#include <QtDebug>
#include <QXmlStreamReader>
#include <motors.h>

static QDomDocument doc;
static QDomElement root;
static QDomNodeList phases;

XmlReaderWriter::XmlReaderWriter()
{
    qDebug() << "XmlReaderWriter run";
    QString errMsg;
    QFileDevice::FileError err = QFileDevice::NoError;

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

    root = doc.firstChildElement();

    phases = root.elementsByTagName("position");

    for(int i = 0; i < phases.count(); i++) {
        QDomNode phaseNode = phases.at(i);
        if(phaseNode.isElement()) {
            QDomElement phase = phaseNode.toElement();
            qDebug() << "ID: " << phase.attribute("id") << ", MOTOR " << phase.attribute("motor")
                     << ", ENCODER " << phase.attribute("encoder") << ", PASSO " << phase.attribute("degrees");
        }
    }
}

void XmlReaderWriter::readElements(QDomElement _root, QString _tag)
{
    QString _id = _root.attribute("id");
    QString _name = _root.attribute("name");

    qDebug() << "Motor id = " << _id << ", Name: " << _name;

    QDomNodeList nodes = _root.elementsByTagName(_tag);
    QString pos[nodes.count()];
    QString degrees[nodes.count()];

    for(int i = 0; i < nodes.count(); i++) {
        QDomNode n = nodes.at(i);
        if(n.isElement()) {
            QDomElement m = n.toElement();
            pos[i] = m.attribute("encoder");
            degrees[i] = m.attribute("degrees");

            qDebug() << "Position " << i << ", gradi encoder: " << pos[i] << ", passo: " << degrees[i];
        }
    }
}

void XmlReaderWriter::writeDoc()
{

}
