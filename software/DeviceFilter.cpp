#include "DeviceFilter.h"

DeviceFilter::DeviceFilter()
    : first(qQNaN()),
      min(qQNaN()),
      max(qQNaN()),
      average(qQNaN()),
      last(qQNaN())
{
}

void DeviceFilter::append(qreal value)
{
    if (qIsFinite(value))
    {
        const qreal factor = 0.75;
        first = qIsFinite(first) ? first : value;
        min = qIsFinite(min) ? qMin(min, value) : value;
        max = qIsFinite(max) ? qMax(max, value) : value;
        average = qIsFinite(average) ? factor * average + (1.0 - factor) * value : value;
    }

    last = value;
}

qreal DeviceFilter::toValue(DeviceFilter::Type parameter) const
{
    switch (parameter)
    {
    case LastType:
        return last;

    case MinValue:
        return min;

    case MaxValue:
        return max;

    case BandValue:
        return max - min;

    case DeviationValue:
        return last - first;

    case AverageValue:
        return average;
    }

    return qQNaN();
}
