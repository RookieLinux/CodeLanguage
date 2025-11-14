//
// Created by rookie on 2025/11/8.
//

#ifndef QML_EXAMPLES_QMLREALTIMEPLOT_H
#define QML_EXAMPLES_QMLREALTIMEPLOT_H

#pragma once

#include <QQuickPaintedItem>
#include "3rdparty/qcustomplot/qcustomplot.h"
#include "3rdparty/qcustomplot/macros.h"
#include "3rdparty/qcustomplot/axis.h"
#include "3rdparty/qcustomplot/grid.h"
#include "3rdparty/qcustomplot/ticker.h"

namespace QmlRealtimePlotNameSpace
{
    class Axis;
    class QmlRealtimePlot : public QQuickPaintedItem {
        Q_OBJECT
        QML_READ_WRITE_NOTIFY_PROPERTY(QColor, backgroundColor)
        QML_READ_NOTIFY_PROPERTY(QmlRealtimePlotNameSpace::Axis *, xAxis)
        QML_READ_NOTIFY_PROPERTY(QmlRealtimePlotNameSpace::Axis *, yAxis)

        Q_PROPERTY(int maxBufferPoints READ maxBufferPoints WRITE setMaxBufferPoints NOTIFY maxBufferChanged)
        Q_PROPERTY(int refreshMs READ refreshMs WRITE setRefreshMs NOTIFY refreshMsChanged)
        Q_PROPERTY(bool paused READ paused WRITE setPaused NOTIFY pausedChanged)

    public:
        explicit QmlRealtimePlot(QQuickItem *parent = nullptr);
        virtual ~QmlRealtimePlot() override;

        void set_backgroundColor(const QColor &value);
        void paint(QPainter *painter) override;
        int maxBufferPoints() const { return m_maxBufferPoints; }
        int refreshMs() const { return m_refreshMs; }
        bool paused() const { return m_paused; }

    public slots:
        void appendBatch(const QVector<double> &x, const QVector<double> &y);
        void setMaxBufferPoints(int n);
        void setRefreshMs(int ms);
        void setPaused(bool p);
        Q_INVOKABLE void clearBuffer();
        Q_INVOKABLE void updateCustomPlotSize() const;

    signals:
        void rangeChanged();
        void maxBufferChanged();
        void refreshMsChanged();
        void pausedChanged();


    private slots:
        void onRepaintTimer();


    private:
        void ensureGraph();
        QCustomPlot *m_customPlot = nullptr;
        QCPGraph *m_graph = nullptr;
        QVector<double> m_xBuf;
        QVector<double> m_yBuf;
        qint64 m_bufHead = 0;
        qint64 m_bufSize = 0;
        double m_xMin = 0, m_xMax = 10, m_yMin = -1, m_yMax = 1;
        int m_maxBufferPoints = 2000000;
        int m_refreshMs = 33;
        bool m_paused = false;
        QTimer m_repaintTimer;
        QReadWriteLock m_bufLock;
    };
}
#endif //QML_EXAMPLES_QMLREALTIMEPLOT_H