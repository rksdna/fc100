#include <QSettings>
#include "Enum.h"
#include "DeviceReference.h"

qreal DeviceReference::minFrequency()
{
    return 9E+6;
}

qreal DeviceReference::maxFrequency()
{
    return 11E+6;
}

DeviceReference::DeviceReference(QObject *parent)
    : QObject(parent),
      m_source(InternalSource),
      m_frequency(10E+6)
{
}

DeviceReference::Source DeviceReference::source() const
{
    return m_source;
}

void DeviceReference::setSource(Source source)
{
    if (m_source != source)
    {
        m_source = source;
        emit sourceChanged(m_source);
    }
}

qreal DeviceReference::frequency() const
{
    return m_frequency;
}

void DeviceReference::setFrequency(qreal frequency)
{
    if (m_frequency != frequency)
    {
        m_frequency = frequency;
        emit frequencyChanged(m_frequency);
    }
}

void DeviceReference::saveToSettings(QSettings &settings) const
{
    settings.setValue("Source", fromEnum(source()));
    settings.setValue("Frequency", frequency());
}

void DeviceReference::restoreFromSettings(QSettings &settings)
{
    setSource(toEnum<Source>(settings.value("Source"), InternalSource));
    setFrequency(settings.value("Frequency").toDouble());
}
