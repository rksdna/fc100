#ifndef CUSTOMTRENDDISPLAY_H
#define CUSTOMTRENDDISPLAY_H

#include "CustomDisplay.h"

class CustomTrendDisplay : public CustomDisplay
{
    Q_OBJECT

public:
    explicit CustomTrendDisplay(QWidget *parent = 0);

    QSize trendSize() const;
    void setTrendSize(const QSize &size);

    QVector<qreal> trend() const;
    void setTrend(const QVector<qreal> &trend);

protected:
    void paintContent(const QRect &content, QPainter &painter) const;
    QSize contentSize() const;

private:
    int map(int top, int bottom, qreal min, qreal value, qreal max) const;
    void updateCache();

private:
    QSize m_trendSize;
    QVector<qreal> m_trend;
    qreal m_min;
    qreal m_max;
};

#endif // CUSTOMTRENDDISPLAY_H
