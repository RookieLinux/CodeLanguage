//
// Created by rookie on 2025/11/8.
//

#include "qmlrealtimeplot.h"

namespace QmlRealtimePlotNameSpace
{
    QmlRealtimePlot::QmlRealtimePlot(QQuickItem *parent): QQuickPaintedItem(parent)
    {
        setFlag(ItemHasContents, true);

        m_customPlot = new QCustomPlot;
        m_xAxis = new QmlRealtimePlotNameSpace::Axis(m_customPlot->xAxis, m_customPlot, this);
        m_yAxis = new QmlRealtimePlotNameSpace::Axis(m_customPlot->yAxis, m_customPlot, this);
        #if defined(QCP_OPENGL)
        m_customPlot->setOpenGl(true);
        #endif
        m_customPlot->setNotAntialiasedElements(QCP::aeAll);//关闭抗锯齿 绘制更快

        ensureGraph();
        connect(this, &QQuickItem::widthChanged, this, &QmlRealtimePlot::updateCustomPlotSize);
        connect(this, &QQuickItem::heightChanged, this, &QmlRealtimePlot::updateCustomPlotSize);
        connect(&m_repaintTimer, &QTimer::timeout, this, &QmlRealtimePlot::onRepaintTimer);
        m_repaintTimer.start(m_refreshMs);
    }
    QmlRealtimePlot::~QmlRealtimePlot()
    {
        delete m_customPlot;
        m_xAxis = nullptr;
        m_yAxis = nullptr;
    }

    void QmlRealtimePlot::set_backgroundColor(const QColor &value)
    {
        if(m_backgroundColor == value) return;
        m_backgroundColor = value;
        m_customPlot->setBackground(QBrush(m_backgroundColor));
        emit backgroundColorChanged(m_backgroundColor);
        m_customPlot->replot();
    }
    void QmlRealtimePlot::ensureGraph()
    {
        if (!m_customPlot) return;
        if (!m_customPlot->graphCount()) m_customPlot->addGraph();
        m_graph = m_customPlot->graph(0);
        m_graph->setLineStyle(QCPGraph::lsStepCenter);
        m_graph->setAdaptiveSampling(true);
        m_graph->setAntialiased(false);
    }
    void QmlRealtimePlot::appendBatch(const QVector<double> &x, const QVector<double> &y)
    {
        if (m_paused) return;
        if (x.isEmpty() || y.isEmpty() || x.size() != y.size()) return;


        QWriteLocker locker(&m_bufLock);
        qint64 addN = x.size();
        qint64 newSize = m_bufSize + addN;
        if (newSize > m_maxBufferPoints) {
            qint64 keep = qint64(m_maxBufferPoints);
            if (keep <= 0) {
                m_xBuf.clear(); m_yBuf.clear(); m_bufSize = 0; m_bufHead = 0;
            } else {
                QVector<double> newX(keep), newY(keep);
                qint64 startIndex = (m_bufHead + (m_bufSize - keep)) % m_bufSize;
                for (qint64 i=0;i<keep;i++) {
                    qint64 idx = (startIndex + i) % m_bufSize;
                    newX[i] = m_xBuf[idx];
                    newY[i] = m_yBuf[idx];
                }
                m_xBuf = std::move(newX);
                m_yBuf = std::move(newY);
                m_bufHead = 0;
                m_bufSize = keep;
                newSize = m_bufSize + addN;
            }
        }


        if (m_xBuf.size() < newSize) {
            m_xBuf.resize(int(newSize));
            m_yBuf.resize(int(newSize));
        }
        qint64 tail = m_bufHead + m_bufSize;
        for (qint64 i=0;i<addN;i++) {
            m_xBuf[tail + i] = x[size_t(i)];
            m_yBuf[tail + i] = y[size_t(i)];
        }
        m_bufSize = newSize;
    }
    void QmlRealtimePlot::onRepaintTimer()
    {
        if (m_paused) return;
        // 只在可见并且 plot 存在时绘制
        if (!m_customPlot) return;
        if (width() <= 0 || height() <= 0) return;

        // 从缓冲复制必要数据到 graph（这里我们使用降采样：按像素采样）
        QReadLocker locker(&m_bufLock);
        if (m_bufSize == 0) return;

        m_xMin = m_xAxis->lower(), m_xMax = m_xAxis->upper(),
        m_yMin = m_yAxis->lower(), m_yMax = m_yAxis->upper();
        // 计算当前需要显示的索引范围（显示最近 m_xRange = xMax-xMin）
        // 找尾部位置
        qint64 tailIndex = (m_bufHead + m_bufSize - 1);
        // 找从尾往前第一个满足 x >= xMin 的位置 —— 简化策略：线性扫描尾到头直到超出显示窗口或达到最大样本数
        double xRangeMin = m_xMin;
        double xRangeMax = m_xMax;
        // 若 xRangeMax <= xRangeMin, fallback 全部
        qint64 startIdx = 0, len = m_bufSize;
        if (xRangeMax > xRangeMin) {
            // 从后向前找到第一个 >= xRangeMin
            qint64 found = -1;
            for (qint64 i = m_bufSize-1; i >= 0; --i) {
                double xv = m_xBuf[(m_bufHead + i) % m_xBuf.size()];
                if (xv < xRangeMin) {
                    found = i+1;
                    break;
                }
                if (i==0) found = 0;
            }
            if (found < 0) found = 0;
            startIdx = found;
            len = m_bufSize - startIdx;
        }

        // 要发送给 QCustomPlot 的点数上限：按像素宽度（每像素最多 1 点）
        int pixelWidth = qMax(1, int(width()));
        int maxPts = pixelWidth * 2; // 放宽一点：每像素最多 2 点
        if (len > maxPts) {
            // 简单降采样：等距抽样
            QVector<double> sx;
            QVector<double> sy;
            sx.reserve(maxPts);
            sy.reserve(maxPts);
            double step = double(len) / double(maxPts);
            for (int i=0;i<maxPts;i++) {
                qint64 idx = startIdx + qFloor(i * step);
                if (idx < 0) idx = 0;
                if (idx >= m_bufSize) idx = m_bufSize - 1;
                qint64 realIdx = (m_bufHead + idx) % m_xBuf.size();
                sx.append(m_xBuf[realIdx]);
                sy.append(m_yBuf[realIdx]);
            }
            locker.unlock(); // 释放读锁
            m_graph->setData(sx, sy, true);
        } else {
            // 直接复制 segment
            QVector<double> sx; sx.reserve(len);
            QVector<double> sy; sy.reserve(len);
            for (qint64 i=0;i<len;i++) {
                qint64 realIdx = (m_bufHead + startIdx + i) % m_xBuf.size();
                sx.append(m_xBuf[realIdx]);
                sy.append(m_yBuf[realIdx]);
            }
            locker.unlock();
            m_graph->setData(sx, sy, true);
        }

        // 更新轴范围（若用户自定义范围则使用，否则自动滚动到最新）
        if (m_xMax <= m_xMin) {
            // 自动滚动：显示尾部一定宽度（例如 10% 的信号）
            // fallback: 用最后一段 x 值
            // 这里我们保留用户可控，若用户设置的范围有效则以用户为准
        } else {
            m_customPlot->xAxis->setRange(m_xMin, m_xMax);
        }
        m_customPlot->yAxis->setRange(m_yMin, m_yMax);

        // 异步重绘
        m_customPlot->replot(QCustomPlot::rpQueuedReplot);

        // 触发 QQuickPaintedItem 更新（paint 中会把 QCustomPlot 绘制到 QPainter）
        update();
    }
    void QmlRealtimePlot::paint(QPainter *painter)
    {
        if (m_customPlot) {
            QPixmap pixmap(boundingRect().size().toSize());
            m_customPlot->render(&pixmap); // Render QCustomPlot to a pixmap
            painter->drawPixmap(0, 0, pixmap); // Draw the pixmap on the QML item
        }
    }
    void QmlRealtimePlot::setMaxBufferPoints(int n) {
        m_maxBufferPoints = qMax(1, n);
        emit maxBufferChanged();
    }
    void QmlRealtimePlot::setRefreshMs(int ms) {
        m_refreshMs = qMax(5, ms);
        m_repaintTimer.start(m_refreshMs);
        emit refreshMsChanged();
    }
    void QmlRealtimePlot::setPaused(bool p) {
        m_paused = p;
        emit pausedChanged();
    }
    //对QML
    void QmlRealtimePlot::clearBuffer() {
        QWriteLocker locker(&m_bufLock);
        m_xBuf.clear();
        m_yBuf.clear();
        m_bufHead = m_bufSize = 0;
        // 清空 graph
        m_graph->data()->clear();
        update();
    }
    void QmlRealtimePlot::updateCustomPlotSize() const {
        if (m_customPlot)
        {
            m_customPlot->setGeometry(x(), y(), width(), height());
        }
    }


}