#include <QJSEngine>
//#include "Format.h"
#include "DeviceProcessor.h"

DeviceProcessor::DeviceProcessor(QObject *parent)
    : QObject(parent)
      /*m_countFormat(new Format(Format::CountType, Format::OneUnit, 0, this)),
      m_timeFormat(new Format(Format::TimeType, Format::MicroUnit, 3, this)),
      m_frequencyFormat(new Format(Format::FrequencyType, Format::MegaUnit, 6, this)),
      m_userFormat(new Format(Format::UserType, Format::OneUnit, 3, this))*/
{
}

/*Format *DeviceProcessor::countFormat() const
{
    return m_countFormat;
}

Format *DeviceProcessor::timeFormat() const
{
    return m_timeFormat;
}

Format *DeviceProcessor::frequencyFormat() const
{
    return m_frequencyFormat;
}

Format *DeviceProcessor::userFormat() const
{
    return m_userFormat;
}*/

void DeviceProcessor::clear()
{

}

/*void DeviceProcessor::take(Format::Type type, qreal value)
{

}*/

void DeviceProcessor::saveToSettings(QSettings &settings)
{

}

void DeviceProcessor::restoreFromSettings(QSettings &settings)
{

}

