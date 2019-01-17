#ifndef SAMPLE_H
#define SAMPLE_H

#include <QtGlobal>

struct Sample
{
    enum Type
    {
        FrequencyType,
        RpmType,
        PeriodType,
        EventsType,
        TimeType
    };

    Sample();
    Sample(qreal counter, qreal timer);

    qreal counter;
    qreal timer;

    qreal toValue(Type type) const;
};

#endif // SAMPLE_H
