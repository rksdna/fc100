#ifndef DEVICEFILTER_H
#define DEVICEFILTER_H

#include <QtGlobal>

struct DeviceFilter
{
    enum Type
    {
        LastType,
        MinValue,
        MaxValue,
        BandValue,
        DeviationValue,
        AverageValue
    };

    DeviceFilter();

    void append(qreal value);
    qreal toValue(Type parameter) const;

    qreal first;
    qreal min;
    qreal max;
    qreal average;
    qreal last;
};

#endif // DEVICEFILTER_H
