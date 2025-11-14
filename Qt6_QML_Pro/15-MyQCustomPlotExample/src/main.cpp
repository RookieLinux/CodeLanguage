#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include "qmlrealtimeplot.h"
#include "datastreamer.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    qmlRegisterType<QmlRealtimePlotNameSpace::QmlRealtimePlot>("RealtimePlot", 1, 0, "RealtimePlot");

    QQmlApplicationEngine engine;
    engine.load(QUrl("qml/main.qml"));
    if (engine.rootObjects().isEmpty()) return -1;

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
        // 连接 streamer.batchReady -> plot.appendBatch（QueuedConnection 保证在主线程）
        QObject::connect(streamer, &DataStreamer::batchReady, plotObj, [plotObj](const QVector<double> &x, const QVector<double> &y){
            // 调用 QML 对象的 appendBatch 槽
            QMetaObject::invokeMethod(plotObj, "appendBatch", Qt::QueuedConnection,
                                      Q_ARG(QVector<double>, x), Q_ARG(QVector<double>, y));
        });
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
