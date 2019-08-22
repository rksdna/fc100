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
        emit ch1DescriptionChanged(description(m_ch1Threshold, m_ch1Probe));
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
        emit ch1DescriptionChanged(description(m_ch1Threshold, m_ch1Probe));
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
        emit ch2DescriptionChanged(description(m_ch2Threshold, m_ch2Probe));
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
        emit ch2DescriptionChanged(description(m_ch2Threshold, m_ch2Probe));
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

QString Device::description(int threshold, Device::Probe probe) const
{
    switch (probe)
    {
    case x1Probe:
        return tr("%1").arg(-5.0 + 10.0 * threshold / 256, 5, 'f', 2, QChar(' '));

    case x10Probe:
        return tr("%1").arg(-50.0 + 100.0 * threshold / 256, 5, 'f', 1, QChar(' '));

    case x100Probe:
        return tr("%1").arg(-500.0 + 1000.0 * threshold / 256, 5, 'f', 0, QChar(' '));

    default:
        break;
    }

    return tr("---");
}
