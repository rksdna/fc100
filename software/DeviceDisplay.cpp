#include "DeviceDisplay.h"

DeviceDisplay::Sample::Sample(int events, qreal time)
    : events(events),
      time(time)
{
}

qreal DeviceDisplay::Sample::toValue(DeviceDisplay::Format format) const
{
    switch (format)
    {
    case Frequency:
        return events / time;

    case Period:
        return time / events;

    case RPM:
        return 60 * events / time;

    case Events:
        return events;

    case Time:
        return time;

    }
}

DeviceDisplay::DeviceDisplay()
{
}

void DeviceDisplay::clear()
{
    m_samples.clear();
}

void DeviceDisplay::update(int events, qreal time)
{
    m_samples.enqueue(Sample(events, time));
    while (m_samples.count() > 10)
        m_samples.dequeue();
}

qreal DeviceDisplay::min(DeviceDisplay::Format format) const
{

}

qreal DeviceDisplay::max(DeviceDisplay::Format format) const
{

}

qreal DeviceDisplay::average(DeviceDisplay::Format format) const
{

}

qreal DeviceDisplay::value(DeviceDisplay::Format format) const
{

}
