#include <QPainter>
#include <QFontMetrics>
#include "CustomBarDisplay.h"

CustomBarDisplay::CustomBarDisplay(QWidget *parent)
    : CustomDisplay(parent),
      m_barSize(5, 50),
      m_min(0.0),
      m_max(0.0)
{
}

QSize CustomBarDisplay::barSize() const
{
    return m_barSize;
}

void CustomBarDisplay::setBarSize(const QSize &size)
{
    if (m_barSize != size)
    {
        m_barSize = size;
        updateGeometry();
        update();
    }
}

QVector<qreal> CustomBarDisplay::bars() const
{
    return m_bars;
}

void CustomBarDisplay::setBars(const QVector<qreal> &bars)
{
    m_bars = bars;
    updateCache();
    updateGeometry();
    update();
}

void CustomBarDisplay::paintContent(const QRect &content, QPainter &painter) const
{
    const QColor mid = palette().color(QPalette::Mid);
    const int tick = 2;

    const int left = content.left();
    const int right = content.right();
    const int top = content.top();
    const int bottom = content.bottom();

    const int width = content.width() - 1;
    const int origin = map(top, bottom, m_min, 0.0, m_max);

    painter.setPen(isEnabled() ? color() : mid);

    for (int i = 0; i < m_bars.count(); i++)
    {
        const int from = left + width * i / m_bars.count();
        const int to = left + width * (i + 1) / m_bars.count();
        const int level = map(top, bottom, m_min, m_bars.value(i), m_max);
        if (level != origin)
        {
            painter.drawLine(from, origin, from, level);
            painter.drawLine(to, level, to, origin);
        }

        painter.drawLine(from, level, to, level);
    }

    painter.setPen(mid);
    painter.drawLine(left, origin, right, origin);
    painter.drawLine(left, origin - tick, left, origin + tick);
    painter.drawLine(right, origin - tick, right, origin + tick);
}

QSize CustomBarDisplay::contentSize() const
{
    return QSize(m_bars.count() * m_barSize.width(), m_barSize.height());
}

int CustomBarDisplay::map(int top, int bottom, qreal min, qreal value, qreal max) const
{
    return qFuzzyCompare(max, min) ? (top + bottom) / 2 : bottom - (bottom - top) * (value - min) / (max - min);
}

void CustomBarDisplay::updateCache()
{
    m_min = 0.0;
    m_max = 0.0;
    foreach (qreal bar, m_bars)
    {
        m_min = qMin(m_min, bar);
        m_max = qMax(m_max, bar);
    }
}
