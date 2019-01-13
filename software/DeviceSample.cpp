#include "DeviceSample.h"

DeviceSample::DeviceSample()
    : counter(0),
      timer(0),
      valid(false)
{
}

DeviceSample::DeviceSample(int counter, qreal timer)
    : counter(counter),
      timer(timer),
      valid(true)
{
}

bool DeviceSample::convert(DeviceSample::Format format, qreal &value) const
{
    if (!valid)
        return false;

    switch (format)
    {
    case FrequencyFormat:
        if (!qFuzzyIsNull(timer))
        {
            value = counter / timer;
            return true;
        }

        break;

    case RpmFormat:
        if (!qFuzzyIsNull(timer))
        {
            value = 60.0 * counter / timer;
            return true;
        }

        break;

    case PeriodFormat:
        if (!qFuzzyIsNull(counter))
        {
            value = timer / counter;
            return true;
        }

        break;

    case EventsFormat:
        value = counter;
        return true;

    case TimeFormat:
        value = timer;
        return true;
    }

    return false;
}
