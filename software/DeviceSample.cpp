#include "DeviceSample.h"

DeviceSample::DeviceSample()
    : counter(qQNaN()),
      timer(qQNaN())
{
}

DeviceSample::DeviceSample(qreal counter, qreal timer)
    : counter(counter),
      timer(timer)
{
}

qreal DeviceSample::toValue(DeviceSample::Type type) const
{
    switch (type)
    {
    case FrequencyType:
        if (qIsFinite(counter) && qIsFinite(timer) && !qFuzzyIsNull(timer))
            return counter / timer;

        break;

    case RpmType:
        if (qIsFinite(counter) && qIsFinite(timer) && !qFuzzyIsNull(timer))
            return 60.0 * counter / timer;

        break;

    case PeriodType:
        if (qIsFinite(counter) && qIsFinite(timer) && !qFuzzyIsNull(counter))
            return timer / counter;

        break;

    case EventsType:
        return counter;

    case TimeType:
        return timer;
    }

    return qQNaN();
}
