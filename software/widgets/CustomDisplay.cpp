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
      m_sample(qQNaN()),
      m_origin(qQNaN()),
      m_min(qQNaN()),
      m_max(qQNaN())
{
}

void CustomDisplay::display(const QList<qreal> &data)
{
    update();
}

void CustomDisplay::display1(qreal val)
{
    m_sample = val;
    if (qIsFinite(m_sample))
    {
        //qreal factor = 0.5;

        m_origin = qIsFinite(m_origin) ? m_origin : m_sample;
        m_min = qIsFinite(m_min) ? qMin(m_min, m_sample) : m_sample;
        m_max = qIsFinite(m_max) ? qMax(m_max, m_sample) : m_sample;
        /*m_smooth = qIsFinite(m_min) ? (1.0 - factor) * m_smooth + factor * m_sample : m_sample;

        m_samples.enqueue(m_sample);
        while (m_samples.count() > m_maxSamplesCount)
            m_samples.dequeue();*/

        m_samples.append(m_sample);
        while (m_samples.count() > 500)
            m_samples.takeFirst();
    }

    update();
}

void CustomDisplay::clear()
{
    m_sample = qQNaN();
    m_origin = qQNaN();
    m_min = qQNaN();
    m_max = qQNaN();

    m_samples.clear();

    update();
}

void CustomDisplay::paintEvent(QPaintEvent *event)
{
    QStylePainter painter(this);
    painter.translate(-0.5, -0.5);
    painter.setRenderHint(QPainter::Antialiasing);

    const QColor textColor = palette().color(QPalette::Mid);
    const QColor valueColor = isEnabled() ? QColor("#A5D785") : textColor;

    QRect content = rect();

    const int he = content.height() / 5;
    const int we = content.width() / 4;

    painter.setPen(textColor);

    painter.drawText(QRect(3 * we, 1 * he, we, he), Qt::AlignTop | Qt::AlignLeft, m_unit);

    painter.drawText(QRect(0 * we, 0 * he, we, he), Qt::AlignVCenter | Qt::AlignRight, tr("MIN"));
    painter.drawText(QRect(0 * we, 2 * he, we, he), Qt::AlignVCenter | Qt::AlignRight, tr("P-P"));
    painter.drawText(QRect(3 * we, 0 * he, we, he), Qt::AlignVCenter | Qt::AlignRight, tr("MAX"));
    painter.drawText(QRect(3 * we, 2 * he, we, he), Qt::AlignVCenter | Qt::AlignRight, tr("DEV"));

    painter.setPen(valueColor);


    QRect r(0 * we, 0 * he, we, he);
    qreal s = 0;
    for (int x = 0; x < r.width(); x++)
    {
        qreal p = r.height() * (m_samples.value(x * 500 / r.width()) - m_min) / (m_max - m_min);
        painter.drawLine(r.x() + x - 1, r.y() + s, r.x() + x, r.y() + p);
        s = p;

    }


    painter.drawText(QRect(0 * we, 1 * he, we, he), Qt::AlignVCenter | Qt::AlignRight, format(m_min));
    painter.drawText(QRect(0 * we, 3 * he, we, he), Qt::AlignVCenter | Qt::AlignRight, format(m_max));
    painter.drawText(QRect(3 * we, 1 * he, we, he), Qt::AlignVCenter | Qt::AlignRight, format(m_max - m_min));
    painter.drawText(QRect(3 * we, 3 * he, we, he), Qt::AlignVCenter | Qt::AlignRight, format(m_sample - m_origin));

    QFont f = font();
    f.setPixelSize(2 * he);

    painter.setFont(f);
    painter.drawText(QRect(1 * we, 1 * he, 2 * we, 2 * he), Qt::AlignVCenter | Qt::AlignRight, format(m_sample));

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

QString CustomDisplay::format(qreal value) const
{
    return qIsFinite(value) ? tr("%1").arg(value, 0, 'f', 3): tr("---");
}
