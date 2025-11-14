//
// Created by rookie on 2025/11/8.
//

// datastreamer.cpp
#include "datastreamer.h"
#include <QThread>
#include <QRandomGenerator>
#include <QtMath>

DataStreamer::DataStreamer(QObject *parent) : QObject(parent) {}

void DataStreamer::startGenerating()
{
    m_running = true;
    const int totalStreamSeconds = 3600; // 示例（长时间）
    const int sampleRate = 24000; // 24k samples/s 假设（可调）
    const int batchSize = 1200; // 每次发送的点数
    double t = 0;
    const double dt = 1.0 / sampleRate;

    while (m_running) {
        QVector<double> xs; xs.reserve(batchSize);
        QVector<double> ys; ys.reserve(batchSize);
        for (int i = 0; i < batchSize; ++i) {
            xs.append(t);
            // 示例信号：混合正弦 + 噪声
            double v = qSin(2*M_PI*1.0*t) + 0.5*qSin(2*M_PI*10.0*t) + (QRandomGenerator::global()->bounded(200)/1000.0 - 0.1);
            ys.append(v);
            t += dt;
        }
        emit batchReady(xs, ys);
        // 控制发送速率：假设 batchSize/samplerate 秒
        QThread::msleep(qMax(1, int(1000.0 * batchSize / sampleRate)));
    }
}

void DataStreamer::stop() { m_running = false; }
