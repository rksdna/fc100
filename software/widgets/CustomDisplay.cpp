#include <QDebug>
#include <QPaintEvent>
#include <QStyleOption>
#include <QStylePainter>
#include "CustomDisplay.h"

CustomDisplay::CustomDisplay(QWidget *parent)
    : QFrame(parent),
      m_margins(20, 20, 20, 20),
      m_decimals(3),
      m_unit(tr("Hz")),
      m_hd(false)
{
}

void CustomDisplay::display(const QList<qreal> &data)
{
    m_hd = !data.isEmpty();
    if (m_hd)
    {
        m_first = data.first();
        m_last = data.last();
        m_min = m_first;
        qDebug() << m_last;
        m_max = m_first;
        m_average = 0.0;
        foreach (qreal value, data)
        {
            m_min = qMin(m_min, value);
            m_max = qMax(m_max, value);
            m_average += value;
        }

        m_average /= data.count();

        m_hystogram.clear();
        int pm = 0;
        foreach (qreal value, data)
        {
            const int bin = 10 * (value - m_min) / (m_max - m_min);
            m_hystogram[bin] += 1;
            pm = qMax(pm, m_hystogram[bin]);
        }


    }
    else
    {

    }

    update();
}

void CustomDisplay::paintEvent(QPaintEvent *event)
{
    QStylePainter painter(this);
    painter.translate(-0.5, -0.5);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawText(rect(), Qt::AlignCenter, tr("%1").arg(m_last));

    event->accept();
}

CustomDisplay::Mode CustomDisplay::mode() const
{
    return m_mode;
}

void CustomDisplay::setMode(const Mode &mode)
{
    m_mode = mode;
    update();
}

QString CustomDisplay::unit() const
{
    return m_unit;
}

void CustomDisplay::setUnit(const QString &unit)
{
    m_unit = unit;
    update();
}

int CustomDisplay::decimals() const
{
    return m_decimals;
}

void CustomDisplay::setDecimals(int decimals)
{
    m_decimals = decimals;

    update();
}

QSize CustomDisplay::sizeHint() const
{
    return minimumSizeHint();
}

QSize CustomDisplay::minimumSizeHint() const
{
    return QSize(200, 100);
}
