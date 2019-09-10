#include <QDebug>
#include <QTimer>
#include <QMetaEnum>
#include <QSettings>
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
      m_measure(false),
      m_delay(false)
{
    m_timer->setSingleShot(true);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &Device::timeout);

    connect(m_processor, &DeviceProcessor::cacheInvalidated, this, &Device::clearThenRestart);
    connect(m_processor, &DeviceProcessor::cacheFilled, this, &Device::stop);

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
    m_processor->clear();
}

void Device::stop()
{
    setTrigger(ManualTrigger);
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

QString Device::portName() const
{
    return m_portName;
}

void Device::setPortName(const QString &name)
{
    if (m_portName != name)
    {
        m_portName = name;
        //emit portNameChanged(m_portName);
    }
}

void Device::saveToSettings(QSettings &settings) const
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

    settings.beginGroup("Processor");
    m_processor->saveToSettings(settings);
    settings.endGroup();


    settings.setValue("trigger", fromEnum(m_trigger));
    settings.setValue("mode", fromEnum(m_mode));
    settings.setValue("countEvent", fromEnum(m_countEvent));
    settings.setValue("startEvent", fromEnum(m_startEvent));
    settings.setValue("stopEvent", fromEnum(m_stopEvent));
    settings.setValue("duration", m_duration);


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

    settings.beginGroup("Processor");
    m_processor->restoreFromSettings(settings);
    settings.endGroup();

    setTrigger(toEnum<Trigger>(settings.value("trigger")));
    setMode(toEnum<Mode>(settings.value("mode")));
    setCountEvent(toEnum<Event>(settings.value("countEvent")));
    setStartEvent(toEnum<Event>(settings.value("startEvent")));
    setStopEvent(toEnum<Event>(settings.value("stopEvent")));
    setDuration(settings.value("duration").toInt());

    setPortName(settings.value("portName").toString());
}

void Device::complete(qreal sample)
{
    m_processor->take(DeviceProcessor::FrequencyType, sample, qQNaN());

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
