#include <QDebug>
#include "DeviceChannel.h"

DeviceChannel::DeviceChannel(int min, int max, qreal minVoltage, qreal maxVoltage)
    : m_min(min),
      m_max(max),
      m_minVoltage(minVoltage),
      m_maxVoltage(maxVoltage),
      m_coupling(DcCoupling),
      m_threshold((min + max + 1) / 2)
{
}

DeviceChannel::Coupling DeviceChannel::coupling() const
{
    return m_coupling;
}

void DeviceChannel::setCoupling(DeviceChannel::Coupling value)
{
    m_coupling = value;
}

int DeviceChannel::minThreshold() const
{
    return m_min;
}

int DeviceChannel::maxThreshold() const
{
    return m_max;
}

int DeviceChannel::threshold() const
{
    return m_threshold;
}

void DeviceChannel::setThreshold(int value)
{
    m_threshold = qBound(m_min, value, m_max);
}

qreal DeviceChannel::thresholdVoltage() const
{
    const qreal factor = (m_maxVoltage - m_minVoltage) / (m_max - m_min);
    return m_minVoltage + factor * (m_threshold - m_min);
}

