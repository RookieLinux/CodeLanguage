//
// Created by rookie on 2025/11/8.
//

#ifndef QML_EXAMPLES_DATASTREAMER_H
#define QML_EXAMPLES_DATASTREAMER_H

#pragma once
#include <QObject>
#include <QVector>


class DataStreamer : public QObject {
    Q_OBJECT
    public:
    explicit DataStreamer(QObject *parent = nullptr);


public slots:
    void startGenerating();
    void stop();


signals:
    void batchReady(const QVector<double> &x, const QVector<double> &y);


private:
    bool m_running = false;
};

#endif //QML_EXAMPLES_DATASTREAMER_H