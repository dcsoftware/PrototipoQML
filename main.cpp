#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <encoder.h>
#include <pigpiocommunication.h>
#include <motors.h>
#include <xmlreaderwriter.h>


int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<Encoder>("Encoder", 1, 0, "Encoder");

    qmlRegisterType<PigpioCommunication>("PigpioCommunication", 1, 0, "PigpioCommunication");

    qmlRegisterType<Motors>("Motors", 1, 0, "Motors");

    qmlRegisterType<XmlReaderWriter>("XmlReaderWriter", 1, 0, "XmlReaderWriter");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
