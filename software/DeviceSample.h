#ifndef DEVICESAMPLE_H
#define DEVICESAMPLE_H

#include <QtGlobal>

struct DeviceSample
{
    enum Type
    {
        FrequencyType,
        RpmType,
        PeriodType,
        EventsType,
        TimeType
    };

    DeviceSample();
    DeviceSample(qreal counter, qreal timer);

    qreal counter;
    qreal timer;

    qreal toValue(Type type) const;
};

#endif // DEVICESAMPLE_H
