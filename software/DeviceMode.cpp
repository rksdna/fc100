#include "DeviceMode.h"

DeviceMode::DeviceMode()
    : m_startEdge(Ch1RisingEdge),
      m_stopEdge(Ch1RisingEdge),
      m_counterEdge(Ch1RisingEdge),
      m_timerClock(InternalClock),
      m_duration(100)
{
}

DeviceMode::Edge DeviceMode::startEdge() const
{
    return m_startEdge;
}

void DeviceMode::setStartEdge(Edge value)
{
    m_startEdge = value;
}

DeviceMode::Edge DeviceMode::stopEdge() const
{
    return m_stopEdge;
}

void DeviceMode::setStopEdge(Edge value)
{
    m_stopEdge = value;
}

DeviceMode::Edge DeviceMode::counterEdge() const
{
    return m_counterEdge;
}

void DeviceMode::setCounterEdge(Edge value)
{
    m_counterEdge = value;
}

DeviceMode::Clock DeviceMode::timerClock() const
{
    return m_timerClock;
}

void DeviceMode::setTimerClock(Clock value)
{
    m_timerClock = value;
}

int DeviceMode::duration() const
{
    return m_duration;
}

void DeviceMode::setDuration(int value)
{
    m_duration = value;
}
