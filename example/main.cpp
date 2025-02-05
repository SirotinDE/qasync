#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "cppApi/CppApi.h"
#include "interactor/WebGetterInteractor.h"

int runApp(int argc, char* argv[]) {
    WebGetterInteractor w;
    CppApi cppApi(&w);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/example/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);
    engine.rootContext()->setContextProperty("cppApi", &cppApi);

    return app.exec();
}

int main(int argc, char *argv[])
{
    auto result = runApp(argc, argv);
    return result;
}
