#include "Sample.h"

Sample::Sample()
    : counter(qQNaN()),
      timer(qQNaN())
{
}

Sample::Sample(qreal counter, qreal timer)
    : counter(counter),
      timer(timer)
{
}

qreal Sample::toValue(Sample::Type type) const
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
