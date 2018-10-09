#include "xmlreaderthread.h"
#include <QDomDocument>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>

static QDomDocument doc;
static QDomElement root;
static QDomNodeList phases;

static QStringList _pos;
static QStringList _enc;
static QStringList _mot;
static QStringList _steps;
static QStringList _dir;

XmlReaderThread::XmlReaderThread(QObject *parent) : QThread(parent)
{

}

XmlReaderThread::~XmlReaderThread()
{
    wait();
}

void XmlReaderThread::run()
{
    qDebug() << "XML THREAD READER RUN";
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
    } else {
        root = doc.firstChildElement();

        phases = root.elementsByTagName("position");

        for(int i = 0; i < phases.count(); i++) {
            QDomNode phaseNode = phases.at(i);
            if(phaseNode.isElement()) {
                QDomElement phase = phaseNode.toElement();
                //qDebug() << "ID: " << phase.attribute("id") << ", MOTOR " << phase.attribute("motor")
                         //<< ", ENCODER " << phase.attribute("encoder") << ", STEPS " << phase.attribute("steps");
                _pos.append(phase.attribute("id"));
                _mot.append(phase.attribute("motor"));
                _enc.append(phase.attribute("encoder"));
                _steps.append(phase.attribute("steps"));
                _dir.append(phase.attribute("dir"));

            }
        }
        emit resultReady(phases.count(), _pos, _enc, _mot, _steps, _dir);
    }
}

