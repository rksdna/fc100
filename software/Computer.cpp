#include "Computer.h"

Computer::Computer(QObject *parent)
    : QObject(parent)
{
    clear();
}

void Computer::clear()
{
    m_first = qQNaN();
    m_min = qQNaN();
    m_max = qQNaN();
    m_average = qQNaN();
    m_last = qQNaN();
}

void Computer::process(qreal value)
{
    if (qIsFinite(value))
    {
        const qreal factor = 0.9;
        m_first = qIsFinite(m_first) ? m_first : value;
        m_min = qIsFinite(m_min) ? qMin(m_min, value) : value;
        m_max = qIsFinite(m_max) ? qMax(m_max, value) : value;
        m_average = qIsFinite(m_average) ? factor * m_average + (1.0 - factor) * value : value;
    }

    m_last = value;
}

qreal Computer::toValue(Computer::Function function) const
{
    switch (function)
    {
    case ActualFunction:
        return m_last;

    case MinFunction:
        return m_min;

    case MaxFunction:
        return m_max;

    case BandFunction:
        return m_max - m_min;

    case DeviationFunction:
        return m_last - m_first;

    case AverageFunction:
        return m_average;
    }

    return qQNaN();
}
