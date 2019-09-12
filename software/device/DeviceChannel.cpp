#include <QSettings>
#include "Enum.h"
#include "DeviceChannel.h"
#include "DeviceProcessor.h"

int DeviceChannel::minThreshold()
{
    return 0;
}

int DeviceChannel::maxThreshold()
{
    return 255;
}

DeviceChannel::DeviceChannel(QObject *parent)
    : QObject(parent),
      m_coupling(DcCoupling),
      m_probe(x1Probe),
      m_threshold(128)
{
}

DeviceChannel::Coupling DeviceChannel::coupling() const
{
    return m_coupling;
}

void DeviceChannel::setCoupling(Coupling coupling)
{
    if (m_coupling != coupling)
    {
        m_coupling = coupling;
        emit couplingChanged(m_coupling);
        emit channelInvalidated();
    }
}

DeviceChannel::Probe DeviceChannel::probe() const
{
    return m_probe;
}

void DeviceChannel::setProbe(Probe probe)
{
    if (m_probe != probe)
    {
        m_probe = probe;
        emit probeChanged(m_probe);
        emit textChanged(text());
    }
}

int DeviceChannel::threshold() const
{
    return m_threshold;
}

void DeviceChannel::setThreshold(int threshold)
{
    if (m_threshold != threshold)
    {
        m_threshold = threshold;
        emit thresholdChanged(m_threshold);
        emit textChanged(text());
        emit channelInvalidated();
    }
}

QString DeviceChannel::text() const
{
    const qreal voltage = -5.0 + 10.0 * m_threshold / 256;
    switch (m_probe)
    {
    case x1Probe:
        return DeviceProcessor::toString(voltage, DeviceProcessor::VoltageType, DeviceProcessor::NoUnit, 2);

    case x10Probe:
        return DeviceProcessor::toString(10 * voltage, DeviceProcessor::VoltageType, DeviceProcessor::NoUnit, 1);

    case x100Probe:
        return DeviceProcessor::toString(100 * voltage, DeviceProcessor::VoltageType, DeviceProcessor::NoUnit, 0);

    default:
        break;
    }

    return tr("---");
}

void DeviceChannel::saveToSettings(QSettings &settings) const
{
    settings.setValue("Coupling", fromEnum(coupling()));
    settings.setValue("Probe", fromEnum(probe()));
    settings.setValue("Threshold", threshold());
}

void DeviceChannel::restoreFromSettings(QSettings &settings)
{
    setCoupling(toEnum<Coupling>(settings.value("Coupling"), DcCoupling));
    setProbe(toEnum<Probe>(settings.value("Probe"), x1Probe));
    setThreshold(settings.value("Threshold", 128).toInt());
}

