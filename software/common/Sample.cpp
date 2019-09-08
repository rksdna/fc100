#include <QtMath>
#include <QSettings>
#include "Enum.h"
#include "Sample.h"

Sample::Format::Item::Item(Sample::Unit unit, int decimals)
    : unit(unit),
      decimals(decimals)
{
}

void Sample::Format::Item::saveToSettings(QSettings &settings) const
{
    settings.setValue("Unit", fromEnum(unit));
    settings.setValue("Decimals", decimals);
}

void Sample::Format::Item::restoreFromSettings(QSettings &settings)
{
    unit = toEnum<Unit>(settings.value("Unit"), NoUnit);
    decimals = settings.value("Decimals", 3).toInt();
}

Sample::Unit Sample::Format::unit(Sample::Type type) const
{
    return m_items.value(type).unit;
}

void Sample::Format::setUnit(Sample::Type type, Sample::Unit unit)
{
    m_items[type].unit = unit;
}

int Sample::Format::decimals(Sample::Type type) const
{
    return m_items.value(type).decimals;
}

void Sample::Format::setDecimals(Sample::Type type, int decimals)
{
    m_items[type].decimals = decimals;
}

void Sample::Format::saveToSettings(QSettings &settings) const
{
    foreach (Type type, knownEnum<Type>())
    {
        settings.beginGroup(fromEnum(type).toString());
        m_items.value(type).saveToSettings(settings);
        settings.endGroup();
    }
}

void Sample::Format::restoreFromSettings(QSettings &settings)
{
    foreach (Type type, knownEnum<Type>())
    {
        settings.beginGroup(fromEnum(type).toString());
        m_items[type].restoreFromSettings(settings);
        settings.endGroup();
    }
}

QString Sample::toString(Sample::Type type)
{
    switch (type)
    {
    case NoType:
        return tr("");

    case TimeType:
        return tr("S");

    case FrequencyType:
        return tr("Hz");

    case VoltageType:
        return tr("V");

    case UserType:
        return tr("U");

    default:
        break;
    }

    return tr("?");
}

QString Sample::toString(Sample::Unit unit)
{
    switch (unit)
    {
    case NanoUnit:
        return tr("n");

    case MicroUnit:
        return tr("u");

    case MilliUnit:
        return tr("m");

    case NoUnit:
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

QString Sample::toString(Sample::Type type, Sample::Unit unit)
{
    return tr("%1%2").arg(toString(type), toString(unit));
}

QString Sample::toString(qreal value, Sample::Type type, Sample::Unit unit, int decimals)
{
    return tr("%1%2").arg(value * qPow(10, -unit), 0, 'f', decimals).arg(toString(type, unit));
}

Sample::Format *Sample::format()
{
    static Format formats;
    return &formats;
}

Sample::Sample(Sample::Type type, qreal value)
    : m_type(type),
      m_value(value)
{
}

QString Sample::toString() const
{
    return toString(format()->unit(m_type), format()->decimals(m_type));
}

QString Sample::toString(Sample::Unit unit, int decimals) const
{
    return toString(m_value, m_type, unit, decimals);
}

Sample::Type Sample::type() const
{
    return m_type;
}

void Sample::setType(Type type)
{
    m_type = type;
}

qreal Sample::value() const
{
    return m_value;
}

void Sample::setValue(qreal value)
{
    m_value = value;
}
