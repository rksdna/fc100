#include <QtMath>
#include <QSettings>
#include <QJSEngine>
#include "Enum.h"
#include "DeviceProcessor.h"

DeviceProcessor::Item::Item(DeviceProcessor::Unit unit, int decimals)
    : unit(unit),
      decimals(decimals)
{
}

QString DeviceProcessor::toString(qreal value, DeviceProcessor::Type type, DeviceProcessor::Unit unit, int decimals)
{
    return tr("%1%2").arg(value * qPow(10, -unit), 0, 'f', decimals).arg(toString(type, unit));
}

int DeviceProcessor::minSize()
{
    return 4;
}

int DeviceProcessor::maxSize()
{
    return 4096;
}

DeviceProcessor::DeviceProcessor(QObject *parent)
    : QObject(parent),
      m_engine(new QJSEngine(this)),
      m_conversion(NoConversion),
      m_expression("x"),
      m_mode(SlidingMode),
      m_size(256),
      m_type(NoType),
      m_time(qQNaN()),
      m_absolute(qQNaN())
{
}

DeviceProcessor::Unit DeviceProcessor::unit(DeviceProcessor::Type type) const
{
    return m_formats.value(type).unit;
}

void DeviceProcessor::setUnit(DeviceProcessor::Type type, DeviceProcessor::Unit unit)
{
    if (m_formats.value(type).unit != unit)
    {
        m_formats[type].unit = unit;

        refresh();
    }
}

int DeviceProcessor::decimals(DeviceProcessor::Type type) const
{
    return m_formats.value(type).decimals;
}

void DeviceProcessor::setDecimals(DeviceProcessor::Type type, int decimals)
{
    if (m_formats.value(type).decimals != decimals)
    {
        m_formats[type].decimals = decimals;

        refresh();
    }
}

DeviceProcessor::Conversion DeviceProcessor::conversion() const
{
    return m_conversion;
}

void DeviceProcessor::setConversion(Conversion function)
{
    if (m_conversion != function)
    {
        m_conversion = function;
        emit conversionChanged(m_conversion);
        emit cacheInvalidated();
    }
}

QString DeviceProcessor::expression() const
{
    return m_expression;
}

void DeviceProcessor::setExpression(const QString &expression)
{
    if (m_expression != expression)
    {
        m_expression = expression;

        if (m_conversion == ExpressionConversion)
            emit cacheInvalidated();
    }
}

DeviceProcessor::Mode DeviceProcessor::mode() const
{
    return m_mode;
}

void DeviceProcessor::setMode(Mode mode)
{
    if (m_mode != mode)
    {
        m_mode = mode;

        fixAndRefresh();
    }
}

int DeviceProcessor::size() const
{
    return m_size;
}

void DeviceProcessor::setSize(int size)
{
    if (m_size != size)
    {
        m_size = size;

        fixAndRefresh();
    }
}

void DeviceProcessor::clear()
{
    m_time = qQNaN();
    m_absolute = qQNaN();
    m_buffer.clear();

    refresh();
}

void DeviceProcessor::take(DeviceProcessor::Type type, qreal value, qreal time)
{
    type = convertType(type);
    value = convertValue(value);

    m_time = time;
    m_absolute = value;

    if (m_type != type)
    {
        m_type = type;
        m_buffer.clear();
    }

    if (qIsFinite(m_absolute))
    {
        switch (m_mode)
        {
        case SlidingMode:
            m_buffer.append(m_absolute);
            while (m_buffer.size() > m_size)
                m_buffer.removeFirst();

            break;

        case CumulativeMode:
            m_buffer.append(m_absolute);
            if (m_buffer.size() >= m_size)
                emit cacheFilled();

            break;
        }
    }

    refresh();
}

QString DeviceProcessor::toString(qreal value, DeviceProcessor::Type type) const
{
    return toString(value, type, m_formats.value(type).unit, m_formats.value(type).decimals);
}

void DeviceProcessor::saveToSettings(QSettings &settings) const
{
    foreach (Type type, knownEnum<Type>())
    {
        settings.beginGroup(fromEnum(type).toString());
        settings.setValue("Unit", fromEnum(unit(type)));
        settings.setValue("Decimals", decimals(type));
        settings.endGroup();
    }

    settings.setValue("Conversion", fromEnum(conversion()));
    settings.setValue("Expression", expression());

    settings.setValue("Mode", fromEnum(mode()));
    settings.setValue("Size", size());
}

void DeviceProcessor::restoreFromSettings(QSettings &settings)
{
    foreach (Type type, knownEnum<Type>())
    {
        settings.beginGroup(fromEnum(type).toString());
        setUnit(type, toEnum<Unit>(settings.value("Unit"), NoUnit));
        setDecimals(type, settings.value("Decimals", 3).toInt());
        settings.endGroup();
    }

    setConversion(toEnum<Conversion>(settings.value("Conversion"), NoConversion));
    setExpression(settings.value("Expression", "x").toString());

    setMode(toEnum<Mode>(settings.value("Mode"), SlidingMode));
    setSize(settings.value("Size", 256).toInt());
}

QString DeviceProcessor::toString(DeviceProcessor::Unit unit, bool prefix)
{
    switch (unit)
    {
    case NanoUnit:
        return prefix ? tr("n") : tr("×10ᐨ⁹");

    case MicroUnit:
        return prefix ? tr("u") : tr("×10ᐨ⁶");

    case MilliUnit:
        return prefix ? tr("m") : tr("×10ᐨ³");

    case NoUnit:
        return tr("");

    case KiloUnit:
        return prefix ? tr("k") : tr("×10³");

    case MegaUnit:
        return prefix ? tr("M") : tr("×10⁶");

    case GigaUnit:
        return prefix ? tr("G") : tr("×10⁹");

    default:
        break;
    }

    return QString();
}

DeviceProcessor::Type DeviceProcessor::convertType(DeviceProcessor::Type type) const
{
    switch (m_conversion)
    {
    case ExpressionConversion:
        return CustomType;

    default:
        break;
    }

    return type;
}

qreal DeviceProcessor::convertValue(qreal value) const
{
    switch (m_conversion)
    {
    case ExpressionConversion:
        m_engine->globalObject().setProperty("x", value);
        return m_engine->evaluate(m_expression).toNumber();

    default:
        break;
    }

    return value;
}

QString DeviceProcessor::toString(DeviceProcessor::Type type, DeviceProcessor::Unit unit)
{
    switch (type)
    {
    case NoType:
        return toString(unit, false);

    case DutyType:
        return tr("%1 %").arg(toString(unit, false));

    case TimeType:
        return tr("%1S").arg(toString(unit, true));;

    case FrequencyType:
        return tr("%1Hz").arg(toString(unit, true));;

    case VoltageType:
        return tr("%1V").arg(toString(unit, true));

    case CustomType:
        return toString(unit, false);

    default:
        break;
    }

    return QString();
}

void DeviceProcessor::fixAndRefresh()
{
    if (m_buffer.size() > m_size && m_mode == SlidingMode)
    {
        m_buffer = m_buffer.mid(m_buffer.size() - m_size, m_size);

        refresh();
    }
}

void DeviceProcessor::refresh()
{
    const QString empty = tr("---");

    if (qIsFinite(m_time))
        emit timeChanged(toString(m_time, TimeType));
    else
        emit timeChanged(empty);

    if (qIsFinite(m_absolute))
    {
        const qreal relative = m_absolute - m_buffer.first();
        emit absoluteChanged(toString(m_absolute, m_type));
        emit relativeChanged(toString(relative, m_type));
    }
    else
    {
        emit absoluteChanged(empty);
        emit relativeChanged(empty);
    }

    if (m_buffer.isEmpty())
    {
        emit minChanged(empty);
        emit maxChanged(empty);
        emit bandChanged(empty);
        emit meanChanged(empty);
        emit standartDeviationChanged(empty);
        emit countChanged(empty);
        emit trendChanged(QVector<qreal>());
        emit barsChanged(QVector<qreal>());
    }
    else
    {
        qreal min = m_buffer.value(0);
        qreal max = m_buffer.value(0);
        qreal sum = 0.0;
        foreach (qreal sample, m_buffer)
        {
            min = qMin(min, sample);
            max = qMax(max, sample);
            sum += sample;
        }

        emit minChanged(toString(min, m_type));
        emit maxChanged(toString(max, m_type));

        const qreal band = max - min;
        emit bandChanged(toString(band, m_type));

        const qreal mean = sum / m_buffer.count();
        emit meanChanged(toString(mean, m_type));

        if (m_buffer.count() > 1)
        {
            qreal sum2 = 0.0;
            foreach (qreal DeviceProcessor, m_buffer)
                sum2 += qPow(DeviceProcessor - mean, 2);

            const qreal deviation = qSqrt(sum2 / (m_buffer.count() - 1));
            emit standartDeviationChanged(toString(deviation, m_type));
        }
        else
        {
            emit standartDeviationChanged(empty);
        }

        const int k = 1 + qLn(m_buffer.count()) / qLn(2);
        QVector<qreal> histogram(k);
        foreach (qreal sample, m_buffer)
        {
            const int i = qBound(0, int(k * (sample - min) / (max - min)), k - 1);
            histogram[i] += 1;
        }

        emit countChanged(toString(m_buffer.count(), DeviceProcessor::NoType));
        emit trendChanged(m_buffer);
        emit barsChanged(histogram);
    }
}
