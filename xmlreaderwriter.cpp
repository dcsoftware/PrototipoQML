#include "xmlreaderwriter.h"
#include <QDomDocument>
#include <QFile>
#include <QtDebug>
#include <QXmlStreamReader>

static QDomDocument doc;
static QDomElement root;
static QDomNodeList phases;

static QStringList _pos;
static QStringList _enc;
static QStringList _mot;
static QStringList _steps;
static QStringList _dir;

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

    readElements();

    file.close();

}

void XmlReaderWriter::readElements()
{
    root = doc.firstChildElement();

    phases = root.elementsByTagName("position");

    for(int i = 0; i < phases.count(); i++) {
        QDomNode phaseNode = phases.at(i);
        if(phaseNode.isElement()) {
            QDomElement phase = phaseNode.toElement();
            qDebug() << "ID: " << phase.attribute("id") << ", MOTOR " << phase.attribute("motor")
                     << ", ENCODER " << phase.attribute("encoder") << ", STEPS " << phase.attribute("steps");
            _pos.append(phase.attribute("id"));
            _mot.append(phase.attribute("motor"));
            _enc.append(phase.attribute("encoder"));
            _steps.append(phase.attribute("steps"));
            _dir.append(phase.attribute("dir"));
        }
    }
}

void XmlReaderWriter::updateElement(int _id, int _degrees, unsigned long _ste)
{
    QFile file("/home/pi/motorsnew.xml");

    if (!file.open(QFile::ReadWrite) || !doc.setContent(&file)) {
        qDebug() << "Failed to open file";
    }

    root = doc.firstChildElement();

    phases = root.elementsByTagName("position");
    QDomNode phaseNode = phases.at(_id);
    if(phaseNode.isElement()) {
        QDomElement phase = phaseNode.toElement();
        qDebug() << "ID: " << phase.attribute("id") << ", MOTOR " << phase.attribute("motor")
                 << ", ENCODER " << phase.attribute("encoder") << ", STEPS " << phase.attribute("steps");
        phase.setAttribute("encoder", QString(_degrees));
        phase.setAttribute("steps", QString::number(_ste));

        file.resize(0);
        QTextStream stream;
        stream.setDevice(&file);
        doc.save(stream, 0);
        file.close();
    }
}

void XmlReaderWriter::updateEncoderPhase(int _id, int _degrees) {
    QFile file("/home/pi/motorsnew.xml");

    if (!file.open(QFile::ReadWrite) || !doc.setContent(&file)) {
        qDebug() << "Failed to open file";
    }

    root = doc.firstChildElement();

    phases = root.elementsByTagName("position");
    QDomNode phaseNode = phases.at(_id);
    if(phaseNode.isElement()) {
        QDomElement phase = phaseNode.toElement();
        qDebug() << "ID: " << phase.attribute("id") << ", MOTOR " << phase.attribute("motor")
                 << ", ENCODER " << phase.attribute("encoder") << ", STEPS " << phase.attribute("steps");
        phase.setAttribute("encoder", QString(_degrees));
        qDebug() << "UPDATE ID: " << phase.attribute("id") << ", MOTOR " << phase.attribute("motor")
                 << ", ENCODER " << phase.attribute("encoder") << ", STEPS " << phase.attribute("steps");
        file.resize(0);
        QTextStream stream;
        stream.setDevice(&file);
        doc.save(stream, 0);
        file.close();
    }
}

void XmlReaderWriter::updateStepPhase(int _id, unsigned long _ste) {
    QFile file("/home/pi/motorsnew.xml");

    if (!file.open(QFile::ReadWrite) || !doc.setContent(&file)) {
        qDebug() << "Failed to open file";
    }

    root = doc.firstChildElement();

    phases = root.elementsByTagName("position");
    QDomNode phaseNode = phases.at(_id);
    if(phaseNode.isElement()) {
        QDomElement phase = phaseNode.toElement();
        qDebug() << "ID: " << phase.attribute("id") << ", MOTOR " << phase.attribute("motor")
                 << ", ENCODER " << phase.attribute("encoder") << ", STEPS " << phase.attribute("steps");
        phase.setAttribute("steps", QString::number(_ste));
        qDebug() << "UPDATE ID: " << phase.attribute("id") << ", MOTOR " << phase.attribute("motor")
                 << ", ENCODER " << phase.attribute("encoder") << ", STEPS " << phase.attribute("steps");
        file.resize(0);
        QTextStream stream;
        stream.setDevice(&file);
        doc.save(stream, 0);
        file.close();
    }
}

QStringList XmlReaderWriter::getPositionArray() {
    return _pos;
}

QStringList XmlReaderWriter::getEncoderArray() {
    return _enc;
}

QStringList XmlReaderWriter::getMotorsArray() {
    return _mot;
}

QStringList XmlReaderWriter::getStepsArray() {
    return _steps;
}

QStringList XmlReaderWriter::getDirArray() {
    return _dir;
}
