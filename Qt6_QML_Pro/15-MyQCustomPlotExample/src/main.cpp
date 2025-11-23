#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include "3rdparty/qcustomplot/realtimeplot.h"
#include "datastreamer.h"

using namespace QmlQCustomPlot;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    qmlRegisterType<RealTimePlot>("RealtimeCustomPlotDemo", 1, 0, "RealTimePlot");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qml/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                Qt::QueuedConnection);
    engine.load(url);

    // 启动后台数据线程
    QThread *workerThread = new QThread;
    DataStreamer *streamer = new DataStreamer;
    streamer->moveToThread(workerThread);

    // 找到 QML 中的 plot 对象
    QObject *root = engine.rootObjects().first();
    QObject *plotObj = root->findChild<QObject*>("realtimePlot");
    if (!plotObj) {
        qWarning() << "Couldn't find realtimePlot object in QML";
    } else {
        qDebug() << "Found object in QML, name is"<< plotObj->objectName();
        // 连接 streamer.batchReady -> plot.appendBatch（QueuedConnection 保证在主线程）
        QObject::connect(streamer, &DataStreamer::batchReady,
            qobject_cast<RealTimePlot*>(plotObj), &RealTimePlot::appendBatch,Qt::UniqueConnection);
    }

    QObject::connect(workerThread, &QThread::started, streamer, &DataStreamer::startGenerating);
    QObject::connect(workerThread, &QThread::finished, streamer, &QObject::deleteLater);

    workerThread->start();

    int ret = app.exec();

    // 优雅停止
    streamer->stop();
    workerThread->quit();
    workerThread->wait();

    return ret;
}
