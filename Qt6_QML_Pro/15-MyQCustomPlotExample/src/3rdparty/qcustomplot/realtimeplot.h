//
// Created by rookie on 2025/11/8.
//

#ifndef REALTIMEPLOT_H
#define REALTIMEPLOT_H

#pragma once

#include <QQuickPaintedItem>
#include <QVariant>
#include "qcustomplot.h"
#include <QMutex>
#include <QMutexLocker>
#include <atomic>
#include "macros.h"
#include "axis.h"
#include "baseplot.h"

namespace QmlQCustomPlot
{
    class Axis;
    class RealTimePlot : public BasePlot {
        Q_OBJECT
        QML_ELEMENT

        Q_PROPERTY(int objIndex READ objIndex WRITE setObjIndex NOTIFY objIndexChanged)
        Q_PROPERTY(int chIndex READ chIndex WRITE setChIndex NOTIFY chIndexChanged)
        Q_PROPERTY(bool decimationEnabled READ decimationEnabled WRITE setDecimationEnabled NOTIFY decimationEnabledChanged)
        Q_PROPERTY(int maxPointsPerFrame READ maxPointsPerFrame WRITE setMaxPointsPerFrame NOTIFY maxPointsPerFrameChanged)
    public:
        explicit RealTimePlot(QQuickItem *parent = nullptr);
        ~RealTimePlot() override;

        int objIndex() const { return m_objIndex; }
        void setObjIndex(int i) {
            if (m_objIndex == i) return;
            m_objIndex = i;
            emit objIndexChanged();
        }
        int chIndex() const { return m_chIndex; }
        void setChIndex(int i) {
            if (m_chIndex == i) return;
            m_chIndex = i;
            //qDebug()<< m_objIndex <<"-"<< m_chIndex;
            emit chIndexChanged();
        }
        bool decimationEnabled() const { return m_decimationEnabled; }
        void setDecimationEnabled(bool v) {
            if (m_decimationEnabled == v) return; m_decimationEnabled = v; emit decimationEnabledChanged(); }
        int maxPointsPerFrame() const { return m_maxPointsPerFrame; }
        void setMaxPointsPerFrame(int v) {
            v = qMax(100, v);
            if (m_maxPointsPerFrame == v) return; m_maxPointsPerFrame = v; emit maxPointsPerFrameChanged(); }
    private:
        void recvMaxBufferPoints(int n);
        // 对输入批次做像素级 min/max 桶化抽稀（已排序 x）
        void decimateMinMax(const QVector<double> &x, const QVector<double> &y,
                            QVector<double> &ox, QVector<double> &oy,
                            int targetPoints,
                            double xLower, double xUpper);
    public slots:
        void appendBatch(const QVector<double> &x, const QVector<double> &y) override;
        void updatePlot() override;

    signals:
        void objIndexChanged();
        void chIndexChanged();
        void decimationEnabledChanged();
        void maxPointsPerFrameChanged();

    private:
        int m_objIndex = -1;
        int m_chIndex = -1;
        // 双缓冲以减少锁竞争：写入后备缓冲，在绘制线程（GUI）中与前台缓冲交换
        QVector<double> m_frontX;
        QVector<double> m_frontY;
        QVector<double> m_backX;
        QVector<double> m_backY;
        QMutex m_bufLock;
        std::atomic<bool> m_hasNewBatch{false};
        bool m_decimationEnabled{true};
        int m_maxPointsPerFrame{0}; // 0 表示按像素宽度×2
    };
}
#endif //REALTIMEPLOT_H