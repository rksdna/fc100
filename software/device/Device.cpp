#include "Device.h"

Device::Device(QObject *parent)
    : QObject(parent),
      m_ch1Coupling(DcCoupling),
      m_ch2Coupling(DcCoupling),
      m_clock(InternalClock)
{
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

Device::Coupling Device::ch1Coupling() const
{
    return m_ch1Coupling;
}

void Device::setCh1Coupling(Coupling coupling)
{
    if (m_ch1Coupling != coupling)
    {
        m_ch1Coupling = coupling;
        emit ch1CouplingChanged(m_ch2Coupling);
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

