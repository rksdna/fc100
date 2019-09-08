#ifndef CUSTOMBARDISPLAY_H
#define CUSTOMBARDISPLAY_H

#include "CustomDisplay.h"

class CustomBarDisplay : public CustomDisplay
{
    Q_OBJECT

public:
    explicit CustomBarDisplay(QWidget *parent = 0);

    QSize barSize() const;
    void setBarSize(const QSize &size);

    QVector<qreal> bars() const;
    void setBars(const QVector<qreal> &bars);

protected:
    void paintContent(const QRect &content, QPainter &painter) const;
    QSize contentSize() const;

private:
    int map(int top, int bottom, qreal min, qreal value, qreal max) const;
    void updateCache();

private:
    QSize m_barSize;
    QVector<qreal> m_bars;
    qreal m_min;
    qreal m_max;
};

#endif // CUSTOMBARDISPLAY_H
