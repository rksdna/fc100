#include "Device.h"

Device::Device(QObject *parent)
    : QObject(parent),
      m_ch1Coupling(DcCoupling),
      m_ch2Coupling(DcCoupling),
      m_ch1Probe(x10Probe),
      m_ch2Probe(x10Probe),
      m_ch1Threshold(128),
      m_ch2Threshold(128),
      m_clock(InternalClock)
{
}

Device::Coupling Device::ch1Coupling() const
{
    return m_ch1Coupling;
}

void Device::setCh1Coupling(Coupling coupling)
{
    if (m_ch1Coupling != coupling)
    {
        m_ch1Coupling = coupling;
        emit ch1CouplingChanged(m_ch1Coupling);
    }
}

Device::Coupling Device::ch2Coupling() const
{
    return m_ch2Coupling;
}

void Device::setCh2Coupling(Coupling coupling)
{
    if (m_ch2Coupling != coupling)
    {
        m_ch2Coupling = coupling;
        emit ch2CouplingChanged(m_ch2Coupling);
    }
}

Device::Probe Device::ch1Probe() const
{
    return m_ch1Probe;
}

void Device::setCh1Probe(Probe probe)
{
    if (m_ch1Probe != probe)
    {
        m_ch1Probe = probe;
        emit ch1ProbeChanged(m_ch1Probe);
    }
}

Device::Probe Device::ch2Probe() const
{
    return m_ch2Probe;
}

void Device::setCh2Probe(Probe probe)
{
    if (m_ch2Probe != probe)
    {
        m_ch2Probe = probe;
        emit ch2ProbeChanged(m_ch2Probe);
    }
}

int Device::ch1Threshold() const
{
    return m_ch1Threshold;
}

void Device::setCh1Threshold(int threshold)
{
    threshold = qBound(0, threshold, 255);
    if (m_ch1Threshold != threshold)
    {
        m_ch1Threshold = threshold;
        emit ch1ThresholdChanged(m_ch1Threshold);
    }
}

int Device::ch2Threshold() const
{
    return m_ch2Threshold;
}

void Device::setCh2Threshold(int threshold)
{
    threshold = qBound(0, threshold, 255);
    if (m_ch2Threshold != threshold)
    {
        m_ch2Threshold = threshold;
        emit ch2ThresholdChanged(m_ch2Threshold);
    }
}

Device::Clock Device::clock() const
{
    return m_clock;
}

void Device::setClock(Clock clock)
{
    if (m_clock != clock)
    {
        m_clock = clock;
        emit clockChanged(m_clock);
    }
}
