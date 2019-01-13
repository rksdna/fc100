#ifndef DEVICESAMPLE_H
#define DEVICESAMPLE_H

#include <QtGlobal>

struct DeviceSample
{
    enum Format
    {
        FrequencyFormat,
        RpmFormat,
        PeriodFormat,
        EventsFormat,
        TimeFormat
    };

    DeviceSample();
    DeviceSample(int counter, qreal timer);

    qreal counter;
    qreal timer;
    bool valid;

    bool convert(Format format, qreal &value) const;
};

#endif // DEVICESAMPLE_H
