#ifndef CUSTOMTRENDDISPLAY_H
#define CUSTOMTRENDDISPLAY_H

#include "CustomDisplay.h"

class CustomTrendDisplay : public CustomDisplay
{
    Q_OBJECT

public:
    explicit CustomTrendDisplay(QWidget *parent = 0);

protected:
    void paintContent(const QRect &content, QPainter &painter) const;
    QSize contentSize() const;

private:
    int CustomBarDisplay::map(int top, int bottom, qreal min, qreal value, qreal max) const
    {
        return qFuzzyCompare(max, min) ? (top + bottom) / 2 : bottom - (bottom - top) * (value - min) / (max - min);
    }

    void updateCache()
    {
        m_min = 0.0;
        m_max = 0.0;
        foreach (qreal point, m_points)
        {
            m_min = qMin(m_min, bar);
            m_max = qMax(m_max, bar);
        }
    }

private:
    QList<qreal> m_points;
    qreal m_min;
    qreal m_max;
};

#endif // CUSTOMTRENDDISPLAY_H
