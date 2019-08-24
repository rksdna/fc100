#include <QDebug>
#include <QObject>
#include <QMetaEnum>
#include <QSettings>
#include <QJSEngine>
#include "Device.h"
#include "MockDevice.h"
#include "DeviceChannel.h"
#include "DeviceReference.h"
#include "DeviceProcessor.h"

template <typename Type>
QVariant fromEnum(Type value)
{
    return QMetaEnum::fromType<Type>().valueToKey(value);
}

template <typename Type>
Type toEnum(const QVariant &text)
{
    bool ok = false;
    const int value = QMetaEnum::fromType<Type>().keyToValue(text.toString().toLatin1().constData(), &ok);
    return ok ? Type(value) : Type();
}

Device *Device::createDevice(const QString &type, QObject *parent)
{
    Q_UNUSED(type)

    return new MockDevice(parent);
}

Device::Device(QObject *parent)
    : QObject(parent),
      m_timer(new QTimer(this)),
      m_engine(new QJSEngine(this)),
      m_channel1(new DeviceChannel(this)),
      m_channel2(new DeviceChannel(this)),
      m_reference(new DeviceReference(this)),
      m_processor(new DeviceProcessor(this)),

      m_trigger(AutoTrigger),
      m_mode(FrequencyMode),
      m_countEventEnabled(true),
      m_countEvent(Ch1RisingEdgeEvent),
      m_startStopEventEnabled(false),
      m_startEvent(Ch1RisingEdgeEvent),
      m_stopEvent(Ch1RisingEdgeEvent),
      m_duration(100),
      m_maxSamplesCount(10),
      m_timeUnit(Second),
      m_timeDecimals(3),
      m_frequencyUnit(Hetz),
      m_frequencyDecimals(3),
      m_functionDecimals(3),
      m_functionEnabled(false),
      m_measure(false),
      m_delay(false)
{
    m_timer->setSingleShot(true);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &Device::timeout);
}

DeviceChannel *Device::channel1() const
{
    return m_channel1;
}

DeviceChannel *Device::channel2() const
{
    return m_channel2;
}

DeviceReference *Device::reference() const
{
    return m_reference;
}

DeviceProcessor *Device::processor() const
{
    return m_processor;
}

void Device::restart()
{
    m_measure = true;
    m_delay = true;
    m_timer->start(m_duration);
    measure();
}

void Device::clear()
{
    emit cleared();

    m_sample = qQNaN();
    m_origin = qQNaN();
    m_min = qQNaN();
    m_max = qQNaN();
    m_smooth = qQNaN();
    m_samples.clear();
}

Device::Trigger Device::trigger() const
{
    return m_trigger;
}

void Device::setTrigger(Trigger trigger)
{
    if (m_trigger != trigger)
    {
        m_trigger = trigger;
        emit triggerChanged(m_trigger);
    }
}

Device::Mode Device::mode() const
{
    return m_mode;
}

void Device::setMode(Mode mode)
{
    if (m_mode != mode)
    {
        switch (m_mode = mode)
        {
        case TimeMode:
            setCountEventEnabled(false);
            setStartStopEventEnabled(true);
            break;

        case FrequencyMode:
        case PeriodMode:
        case CountMode:
        case DutyMode:
            setCountEventEnabled(true);
            setStartStopEventEnabled(false);
            break;

        case GatePeriodMode:
        case GateFrequencyMode:
        case GateCountMode:
            setCountEventEnabled(true);
            setStartStopEventEnabled(true);
            break;

        default:
            break;
        }

        emit modeChanged(m_mode);

        clearThenRestart();
    }
}

Device::Event Device::countEvent() const
{
    return m_countEvent;
}

void Device::setCountEvent(Event event)
{
    if (m_countEvent != event)
    {
        m_countEvent = event;
        emit countEventChanged(m_countEvent);
    }
}

Device::Event Device::startEvent() const
{
    return m_startEvent;
}

void Device::setStartEvent(Event event)
{
    if (m_startEvent != event)
    {
        m_startEvent = event;
        emit startEventChanged(m_startEvent);
    }
}

Device::Event Device::stopEvent() const
{
    return m_stopEvent;
}

void Device::setStopEvent(Event event)
{
    if (m_stopEvent != event)
    {
        m_stopEvent = event;
        emit stopEventChanged(m_stopEvent);
    }
}

int Device::duration() const
{
    return m_duration;
}

void Device::setDuration(int duration)
{
    duration = qBound(0, duration, 10000);
    if (m_duration != duration)
    {
        m_duration = duration;
        emit durationChanged(m_duration);
    }
}

int Device::maxSamplesCount() const
{
    return m_maxSamplesCount;
}

void Device::setMaxSamplesCount(int count)
{
    count = qBound(5, count, 50);
    if (m_maxSamplesCount != count)
    {
        m_maxSamplesCount = count;
        emit maxSamplesCountChanged(m_maxSamplesCount);

        //....
    }
}

Device::TimeUnit Device::timeUnit() const
{
    return m_timeUnit;
}

void Device::setTimeUnit(TimeUnit unit)
{
    if (m_timeUnit != unit)
    {
        m_timeUnit = unit;
        emit timeUnitChanged(m_timeUnit);
    }
}

int Device::timeDecimals() const
{
    return m_timeDecimals;
}

void Device::setTimeDecimals(int decimals)
{
    decimals = qBound(0, decimals, 9);
    if (m_timeDecimals != decimals)
    {
        m_timeDecimals = decimals;
        emit timeDecimalsChanged(m_timeDecimals);
    }
}

Device::FrequencyUnit Device::frequencyUnit() const
{
    return m_frequencyUnit;
}

void Device::setFrequencyUnit(FrequencyUnit unit)
{
    if (m_frequencyUnit != unit)
    {
        m_frequencyUnit = unit;
        emit frequencyUnitChanged(m_frequencyUnit);
    }
}

int Device::frequencyDecimals() const
{
    return m_frequencyDecimals;
}

void Device::setFrequencyDecimals(int decimals)
{
    if (m_frequencyDecimals != decimals)
    {
        m_frequencyDecimals = decimals;
        emit frequencyDecimalsChanged(m_frequencyDecimals);
    }
}

QString Device::function() const
{
    return m_function;
}

void Device::setFunction(const QString &function)
{
    if (m_function != function)
    {
        m_function = function;
        emit functionChanged(m_function);

        clearThenRestart();
    }
}

QString Device::functionUnit() const
{
    return m_functionUnit;
}

void Device::setFunctionUnit(const QString &unit)
{
    if (m_functionUnit != unit)
    {
        m_functionUnit = unit;
        emit functionUnitChanged(m_functionUnit);
    }
}

int Device::functionDecimals() const
{
    return m_functionDecimals;
}

void Device::setFunctionDecimals(int decimals)
{
    if (m_functionDecimals != decimals)
    {
        m_functionDecimals = decimals;
        emit functionDecimalsChanged(decimals);
    }
}

bool Device::isFunctionEnabled() const
{
    return m_functionEnabled;
}

void Device::setFunctionEnabled(bool enabled)
{
    if (m_functionEnabled != enabled)
    {
        m_functionEnabled = enabled;
        emit functionEnabledChanged(m_functionEnabled);

        clearThenRestart();
    }
}

QString Device::portName() const
{
    return m_portName;
}

void Device::setPortName(const QString &name)
{
    if (m_portName != name)
    {
        m_portName = name;
        emit portNameChanged(m_portName);
    }
}

QList<qreal> Device::samples() const
{
    return m_samples;
}

void Device::saveToSettings(QSettings &settings)
{
    settings.beginGroup("Channel1");
    m_channel1->saveToSettings(settings);
    settings.endGroup();

    settings.beginGroup("Channel2");
    m_channel2->saveToSettings(settings);
    settings.endGroup();

    settings.beginGroup("Reference");
    m_reference->saveToSettings(settings);
    settings.endGroup();

    settings.setValue("trigger", fromEnum(m_trigger));
    settings.setValue("mode", fromEnum(m_mode));
    settings.setValue("countEvent", fromEnum(m_countEvent));
    settings.setValue("startEvent", fromEnum(m_startEvent));
    settings.setValue("stopEvent", fromEnum(m_stopEvent));
    settings.setValue("duration", m_duration);

    settings.setValue("maxSamplesCount", m_maxSamplesCount);
    settings.setValue("timeUnit", fromEnum(m_timeUnit));
    settings.setValue("timeDecimals", m_timeDecimals);
    settings.setValue("frequencyUnit", fromEnum(m_frequencyUnit));
    settings.setValue("frequencyDecimals", m_frequencyDecimals);
    settings.setValue("function", m_function);
    settings.setValue("functionUnit", m_functionUnit);
    settings.setValue("functionDecimals", m_functionDecimals);
    settings.setValue("functionEnabled", m_functionEnabled);

    settings.setValue("portName", m_portName);
}

void Device::restoreFromSettings(QSettings &settings)
{
    settings.beginGroup("Channel1");
    m_channel1->restoreFromSettings(settings);
    settings.endGroup();

    settings.beginGroup("Channel2");
    m_channel2->restoreFromSettings(settings);
    settings.endGroup();

    settings.beginGroup("Reference");
    m_reference->restoreFromSettings(settings);
    settings.endGroup();


    setTrigger(toEnum<Trigger>(settings.value("trigger")));
    setMode(toEnum<Mode>(settings.value("mode")));
    setCountEvent(toEnum<Event>(settings.value("countEvent")));
    setStartEvent(toEnum<Event>(settings.value("startEvent")));
    setStopEvent(toEnum<Event>(settings.value("stopEvent")));
    setDuration(settings.value("duration").toInt());

    setMaxSamplesCount(settings.value("maxSamplesCount").toInt());
    setTimeUnit(toEnum<TimeUnit>(settings.value("timeUnit")));
    setTimeDecimals(settings.value("timeDecimals").toInt());
    setFrequencyUnit(toEnum<FrequencyUnit>(settings.value("frequencyUnit")));
    setFrequencyDecimals(settings.value("frequencyDecimals").toInt());
    setFunction(settings.value("function").toString());
    setFunctionUnit(settings.value("functionUnit").toString());
    setFunctionDecimals(settings.value("functionDecimals").toInt());
    setFunctionEnabled(settings.value("functionEnabled").toBool());

    setPortName(settings.value("portName").toString());
}

void Device::complete(qreal sample)
{
    m_sample = function1(sample);
    if (qIsFinite(m_sample))
    {
        qreal factor = 0.5;

        m_origin = qIsFinite(m_origin) ? m_origin : m_sample;
        m_min = qIsFinite(m_min) ? qMin(m_min, sample) : m_sample;
        m_max = qIsFinite(m_min) ? qMax(m_max, sample) : m_sample;
        m_smooth = qIsFinite(m_min) ? (1.0 - factor) * m_smooth + factor * m_sample : m_sample;

        m_samples.enqueue(m_sample);
        while (m_samples.count() > m_maxSamplesCount)
            m_samples.dequeue();
    }

    emit sampleArr(m_sample);

    m_measure = false;
    if (m_trigger == AutoTrigger && !m_delay)
        restart();
}

void Device::timeout()
{
    m_delay = false;
    if (m_trigger == AutoTrigger && !m_measure)
        restart();
}

void Device::clearThenRestart()
{
    clear();

    if (m_measure)
        restart();
}

void Device::setCountEventEnabled(bool enabled)
{
    if (m_countEventEnabled != enabled)
    {
        m_countEventEnabled = enabled;
        emit countEventEnabled(m_countEventEnabled);
    }
}

void Device::setStartStopEventEnabled(bool enabled)
{
    if (m_startStopEventEnabled != enabled)
    {
        m_startStopEventEnabled = enabled;
        emit startStopEventEnabled(m_startStopEventEnabled);
    }
}

qreal Device::function1(qreal sample)
{
    if (qIsFinite(sample) && m_functionEnabled)
    {
        m_engine->globalObject().setProperty("x", sample);
        return m_engine->evaluate(m_function).toNumber();
    }

    return sample;
}
