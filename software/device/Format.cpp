#include <QtMath>
#include <QSettings>
#include "Enum.h"
#include "Format.h"

QString Format::formatType(Format::Type type)
{
    switch (type)
    {
    case Format::CountType:
        return tr("");

    case Format::TimeType:
        return tr("S");

    case Format::FrequencyType:
        return tr("Hz");

    case Format::VoltageType:
        return tr("V");

    case Format::UserType:
        return tr("X");

    default:
        break;
    }

    return tr("?");
}

QString Format::formatUnit(Format::Unit unit)
{
    switch (unit)
    {
    case NanoUnit:
        return tr("n");

    case MicroUnit:
        return tr("u");

    case MilliUnit:
        return tr("m");

    case OneUnit:
        return tr("");

    case KiloUnit:
        return tr("k");

    case MegaUnit:
        return tr("M");

    case GigaUnit:
        return tr("G");

    default:
        break;
    }

    return tr("?");
}

QString Format::suffix(Format::Unit unit, Format::Type type)
{
    return tr("%1%2").arg(formatUnit(unit), formatType(type));
}

QString Format::format(qreal value, int decimals, Format::Unit unit, Format::Type type)
{
    return tr("%1%2").arg(value * qPow(10, -unit), 0, 'f', decimals).arg(suffix(unit, type));
}

int Format::minDecimals()
{
    return 0;
}

int Format::maxDecimals()
{
    return 6;
}

Format::Format(Type type, Unit unit, int decimals, QObject *parent)
    : QObject(parent),
      m_type(type),
      m_unit(unit),
      m_decimals(decimals)
{
}

Format::Type Format::type() const
{
    return m_type;
}

Format::Unit Format::unit() const
{
    return m_unit;
}

void Format::setUnit(Unit unit)
{
    if (m_unit != unit)
    {
        m_unit = unit;
        emit unitChanged(m_unit);
    }
}

int Format::decimals() const
{
    return m_decimals;
}

void Format::setDecimals(int decimals)
{
    if (m_decimals != decimals)
    {
        m_decimals = decimals;
        emit decimalsChanged(m_decimals);
    }
}

QString Format::suffix() const
{
    return suffix(m_unit, m_type);
}

QString Format::suffix(Format::Unit unit) const
{
    return suffix(unit, m_type);
}

QString Format::format(qreal value) const
{
    return format(value, m_decimals, m_unit, m_type);
}

void Format::saveToSettings(QSettings &settings)
{
    settings.setValue("Unit", fromEnum(unit()));
    settings.setValue("Decimals", decimals());
}

void Format::restoreFromSettings(QSettings &settings)
{
    setUnit(toEnum<Unit>(settings.value("Unit"), OneUnit));
    setDecimals(settings.value("Decimals").toInt());
}

