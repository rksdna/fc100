#include <QPainter>
#include "CustomTrendDisplay.h"

CustomTrendDisplay::CustomTrendDisplay(QWidget *parent)
    : CustomDisplay(parent),
      m_trendSize(100, 50)
{
}

QSize CustomTrendDisplay::trendSize() const
{
    return m_trendSize;
}

void CustomTrendDisplay::setTrendSize(const QSize &size)
{
    if (m_trendSize != size)
    {
        m_trendSize = size;
        updateGeometry();
        update();
    }
}

QVector<qreal> CustomTrendDisplay::trend() const
{
    return m_trend;
}

void CustomTrendDisplay::setTrend(const QVector<qreal> &trend)
{
    m_trend = trend;
    updateCache();
    update();
}

void CustomTrendDisplay::paintContent(const QRect &content, QPainter &painter) const
{
    const QColor mid = palette().color(QPalette::Mid);
    const int tick = 2;

    const int left = content.left();
    const int right = content.right();
    const int top = content.top();
    const int bottom = content.bottom();

    const int width = content.width() - 1;
    const int origin = qBound(top, map(top, bottom, m_min, 0.0, m_max), bottom);

    painter.setPen(isEnabled() ? color() : mid);

    const int step = qMax(m_trend.count() / content.width(), 1);
    for (int i = 0; i < m_trend.count() - step; i += step)
    {
        const int from = left + width * i / m_trend.count();
        const int to = left + width * (i + step) / m_trend.count();
        const int level = map(top, bottom, m_min, m_trend.value(i), m_max);
        const int next = map(top, bottom, m_min, m_trend.value(i + step), m_max);

        painter.drawLine(from, level, to, next);
    }

    painter.setPen(mid);
    painter.drawLine(left, origin, right, origin);
    painter.drawLine(left, origin - tick, left, origin + tick);
    painter.drawLine(right, origin - tick, right, origin + tick);
}

QSize CustomTrendDisplay::contentSize() const
{
    return m_trendSize;
}

int CustomTrendDisplay::map(int top, int bottom, qreal min, qreal value, qreal max) const
{
    return qFuzzyCompare(max, min) ? (top + bottom) / 2 : bottom - (bottom - top) * (value - min) / (max - min);
}

void CustomTrendDisplay::updateCache()
{
    m_min = m_trend.value(0);
    m_max = m_trend.value(0);
    foreach (qreal point, m_trend)
    {
        m_min = qMin(m_min, point);
        m_max = qMax(m_max, point);
    }
}
