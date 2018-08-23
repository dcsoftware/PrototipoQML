#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <encoder.h>
#include <i2ccom.h>
#include <pigpiocommunication.h>


int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<Encoder>("Encoder", 1, 0, "Encoder");

    qmlRegisterType<I2CCom>("I2CCom", 1, 0, "I2CCom");

    qmlRegisterType<PigpioCommunication>("PigpioCommunication", 1, 0, "PigpioCommunication");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
