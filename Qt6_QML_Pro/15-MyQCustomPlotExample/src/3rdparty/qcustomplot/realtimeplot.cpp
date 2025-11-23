//
// Created by rookie on 2025/11/8.
//

#include "realtimeplot.h"
#include "qcustomplot.h"
 #include <algorithm>
 #include <limits>
//Q_DECLARE_METATYPE(QCPRange)

namespace QmlQCustomPlot
{
    RealTimePlot::RealTimePlot(QQuickItem *parent): BasePlot(parent)
    {
        connect(this, &BasePlot::maxBufferChanged, this, [this](){
            this->recvMaxBufferPoints(this->maxBufferPoints());
        });
    }
    RealTimePlot::~RealTimePlot()
    {

    }
    void RealTimePlot::recvMaxBufferPoints(int n)
    {
        // 预留前后缓冲容量，避免频繁分配
        m_frontX.reserve(n);
        m_frontY.reserve(n);
        m_backX.reserve(n);
        m_backY.reserve(n);
    }

    void RealTimePlot::appendBatch(const QVector<double> &x, const QVector<double> &y)
    {
        if (paused()) return;
        if (width() <= 0 || height() <= 0) return;
        if (x.isEmpty() || y.isEmpty()) return;

        QMutexLocker locker(&m_bufLock);
        // 后台缓冲赋值，尽量利用已预留容量
        m_backX = x;
        m_backY = y;
        m_hasNewBatch.store(true, std::memory_order_release);
    }

    void RealTimePlot::updatePlot()
    {
        QCustomPlot * m_customPlot = customPlot();
        // 只在可见并且 plot 存在时绘制
        if (!m_customPlot) return;

        // 如果没有新数据，避免无效绘制
        if (!m_hasNewBatch.load(std::memory_order_acquire)) {
            return;
        }

        // 交换前后缓冲，缩短锁时间
        {
            QMutexLocker locker(&m_bufLock);
            if (m_hasNewBatch.load(std::memory_order_relaxed)) {
                m_frontX.swap(m_backX);
                m_frontY.swap(m_backY);
                m_hasNewBatch.store(false, std::memory_order_release);
            }
        }

        // 数据抽稀：按可见范围与像素宽度进行 min/max 桶化
        const auto &gmap = graphsMap();
        if (m_decimationEnabled) {
            const QCPRange xr = m_customPlot->xAxis->range();
            const double xLower = xr.lower;
            const double xUpper = xr.upper;
            // 目标点数：默认按像素宽度的 2 倍（min+max），也可外部设置上限
            int bins = m_maxPointsPerFrame > 0 ? m_maxPointsPerFrame : qMax(100, int(width()));
            QVector<double> ox; ox.reserve(bins * 2);
            QVector<double> oy; oy.reserve(bins * 2);
            decimateMinMax(m_frontX, m_frontY, ox, oy, bins, xLower, xUpper);
            for (auto it = gmap.constBegin(); it != gmap.constEnd(); ++it) {
                it.value()->setData(ox, oy, true);
            }
        } else {
            for (auto it = gmap.constBegin(); it != gmap.constEnd(); ++it) {
                it.value()->setData(m_frontX, m_frontY, true);
            }
        }
        m_customPlot->replot(QCustomPlot::rpQueuedReplot);
    }

    // 像素级 min/max 桶化：x 需要基本递增（时间序）
    void RealTimePlot::decimateMinMax(const QVector<double> &x, const QVector<double> &y,
                                      QVector<double> &ox, QVector<double> &oy,
                                      int targetBins,
                                      double xLower, double xUpper)
    {
        ox.clear(); oy.clear();
        const int n = qMin(x.size(), y.size());
        if (n == 0 || xUpper <= xLower) return;

        // 使用二分查找定位可见范围索引
        const double *xp = x.constData();
        auto lowerIt = std::lower_bound(xp, xp + n, xLower);
        int startIdx = int(lowerIt - xp);
        auto upperIt = std::upper_bound(xp + startIdx, xp + n, xUpper);
        int endIdx = int(upperIt - xp); // [startIdx, endIdx)
        if (endIdx - startIdx <= 0) return;

        const int visibleCount = endIdx - startIdx;
        // 如果可见点数已不多，直接返回
        if (visibleCount <= targetBins * 2) {
            ox.reserve(visibleCount);
            oy.reserve(visibleCount);
            for (int i = startIdx; i < endIdx; ++i) { ox.append(x[i]); oy.append(y[i]); }
            return;
        }

        // 计算每个桶覆盖的 x 范围
        targetBins = qMax(1, targetBins);
        const double span = xUpper - xLower;
        const double binWidth = span / double(targetBins);

        int idx = startIdx;
        for (int b = 0; b < targetBins && idx < endIdx; ++b) {
            const double bx0 = xLower + b * binWidth;
            const double bx1 = (b == targetBins - 1) ? xUpper : (bx0 + binWidth);

            // 聚合当前桶内的点
            double minVal = std::numeric_limits<double>::infinity();
            double maxVal = -std::numeric_limits<double>::infinity();
            int minIdx = -1, maxIdx = -1;

            // 跳过桶前的点（理论上不会有 < bx0 的，因为使用了 lower_bound，但留作稳健）
            while (idx < endIdx && x[idx] < bx0) ++idx;
            const int bucketStart = idx;
            while (idx < endIdx && x[idx] < bx1) {
                const double vy = y[idx];
                if (vy < minVal) { minVal = vy; minIdx = idx; }
                if (vy > maxVal) { maxVal = vy; maxIdx = idx; }
                ++idx;
            }
            const int bucketEnd = idx;
            if (bucketEnd <= bucketStart) continue; // 桶内没有点

            // 按时间顺序输出 min 与 max，保持折线形状
            if (minIdx <= maxIdx) {
                ox.append(x[minIdx]); oy.append(minVal);
                if (maxIdx != minIdx) { ox.append(x[maxIdx]); oy.append(maxVal); }
            } else {
                ox.append(x[maxIdx]); oy.append(maxVal);
                ox.append(x[minIdx]); oy.append(minVal);
            }
        }
    }

}