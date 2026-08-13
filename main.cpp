#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "screenstreamer.h"
#include "imageprovider.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    LiveImageProvider *imageProvider = new LiveImageProvider();
    engine.addImageProvider(QStringLiteral("live"), imageProvider);

    ScreenStreamer streamer(imageProvider);
    engine.rootContext()->setContextProperty("screenStreamer", &streamer);

    engine.loadFromModule("QtMirrorApp", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}