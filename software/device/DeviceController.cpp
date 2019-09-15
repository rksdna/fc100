#include <QSettings>
#include "Enum.h"
#include "DeviceController.h"

DeviceController::DeviceController(QObject *parent)
    : QObject(parent),
      m_trigger(AutoTrigger),
      m_mode(FrequencyMode),
      m_countEvent(Ch1RisingEdgeEvent),
      m_startEvent(Ch1RisingEdgeEvent),
      m_stopEvent(Ch1RisingEdgeEvent),
      m_duration(100)
{
}

DeviceController::Trigger DeviceController::trigger() const
{
    return m_trigger;
}

void DeviceController::setTrigger(Trigger trigger)
{
    if (m_trigger != trigger)
    {
        m_trigger = trigger;
        emit triggerChanged(m_trigger);
    }
}

DeviceController::Mode DeviceController::mode() const
{
    return m_mode;
}

void DeviceController::setMode(Mode mode)
{
    if (m_mode != mode)
    {
        m_mode = mode;

        emit modeChanged(m_mode);
        emit countEventEnabled(isCountEventEnabled());
        emit startStopEventEnabled(isStartStopEventEnabled());

        emit controlInvalidated();
    }
}

bool DeviceController::isCountEventEnabled() const
{
    switch (m_mode)
    {
    case FrequencyMode:
    case PeriodMode:
    case CountMode:
    case DutyMode:
    case GatePeriodMode:
    case GateFrequencyMode:
    case GateCountMode:
        return true;

    default:
        break;
    }

    return false;
}

DeviceController::Event DeviceController::countEvent() const
{
    return m_countEvent;
}

DeviceController::Event DeviceController::complementCountEvent() const
{
    switch (m_countEvent)
    {
    case Ch1RisingEdgeEvent:
        return Ch1FallingEdgeEvent;

    case Ch1FallingEdgeEvent:
        return Ch1RisingEdgeEvent;

    case Ch2RisingEdgeEvent:
        return Ch2FallingEdgeEvent;

    case Ch2FallingEdgeEvent:
        return Ch2FallingEdgeEvent;

    default:
        break;
    }

    return Ch1RisingEdgeEvent;
}


void DeviceController::setCountEvent(Event event)
{
    if (m_countEvent != event)
    {
        m_countEvent = event;
        emit countEventChanged(m_countEvent);
    }
}

bool DeviceController::isStartStopEventEnabled() const
{
    switch (m_mode)
    {
    case TimeMode:
    case GatePeriodMode:
    case GateFrequencyMode:
    case GateCountMode:
        return true;

    default:
        break;
    }

    return false;
}

DeviceController::Event DeviceController::startEvent() const
{
    return m_startEvent;
}

void DeviceController::setStartEvent(Event event)
{
    if (m_startEvent != event)
    {
        m_startEvent = event;
        emit startEventChanged(m_startEvent);
    }
}

DeviceController::Event DeviceController::stopEvent() const
{
    return m_stopEvent;
}

void DeviceController::setStopEvent(Event event)
{
    if (m_stopEvent != event)
    {
        m_stopEvent = event;
        emit stopEventChanged(m_stopEvent);
    }
}

int DeviceController::duration() const
{
    return m_duration;
}

void DeviceController::setDuration(int duration)
{
    if (m_duration != duration)
    {
        m_duration = duration;
        emit durationChanged(m_duration);
    }
}

QColor DeviceController::color() const
{
    return m_color;
}

void DeviceController::setColor(const QColor &color)
{
    if (m_color != color)
    {
        m_color = color;
        emit colorChanged(m_color);
    }
}

void DeviceController::saveToSettings(QSettings &settings) const
{
    settings.setValue("Trigger", fromEnum(trigger()));
    settings.setValue("Mode", fromEnum(mode()));
    settings.setValue("CountEvent", fromEnum(countEvent()));
    settings.setValue("StartEvent", fromEnum(startEvent()));
    settings.setValue("StopEvent", fromEnum(stopEvent()));
    settings.setValue("Duration", duration());
    settings.setValue("Color", color());
}

void DeviceController::restoreFromSettings(QSettings &settings)
{
    setTrigger(toEnum<Trigger>(settings.value("Trigger"), AutoTrigger));
    setMode(toEnum<Mode>(settings.value("Mode"), FrequencyMode));
    setCountEvent(toEnum<Event>(settings.value("CountEvent"), Ch1RisingEdgeEvent));
    setStartEvent(toEnum<Event>(settings.value("StartEvent"), Ch1RisingEdgeEvent));
    setStopEvent(toEnum<Event>(settings.value("StopEvent"), Ch1RisingEdgeEvent));
    setDuration(settings.value("Duration", 100).toInt());
    setColor(settings.value("Color", QColor(Qt::red)).value<QColor>());
}
