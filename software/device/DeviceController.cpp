#include <QSettings>
#include "Enum.h"
#include "DeviceController.h"

DeviceController::DeviceController(QObject *parent)
    : QObject(parent),
      m_trigger(AutoTrigger),
      m_mode(FrequencyMode),
      m_countEventEnabled(true),
      m_countEvent(Ch1RisingEdgeEvent),
      m_startStopEventEnabled(false),
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
        emit controlInvalidated();
    }
}

DeviceController::Event DeviceController::countEvent() const
{
    return m_countEvent;
}

void DeviceController::setCountEvent(Event event)
{
    if (m_countEvent != event)
    {
        m_countEvent = event;
        emit countEventChanged(m_countEvent);
    }
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

void DeviceController::saveToSettings(QSettings &settings) const
{
    settings.setValue("Trigger", fromEnum(trigger()));
    settings.setValue("Mode", fromEnum(mode()));
    settings.setValue("CountEvent", fromEnum(countEvent()));
    settings.setValue("StartEvent", fromEnum(startEvent()));
    settings.setValue("StopEvent", fromEnum(stopEvent()));
    settings.setValue("Duration", duration());
}

void DeviceController::restoreFromSettings(QSettings &settings)
{
    setTrigger(toEnum<Trigger>(settings.value("Trigger"), AutoTrigger));
    setMode(toEnum<Mode>(settings.value("Mode"), FrequencyMode));
    setCountEvent(toEnum<Event>(settings.value("CountEvent"), Ch1RisingEdgeEvent));
    setStartEvent(toEnum<Event>(settings.value("StartEvent"), Ch1RisingEdgeEvent));
    setStopEvent(toEnum<Event>(settings.value("StopEvent"), Ch1RisingEdgeEvent));
    setDuration(settings.value("Duration", 100).toInt());
}

void DeviceController::setCountEventEnabled(bool enabled)
{
    if (m_countEventEnabled != enabled)
    {
        m_countEventEnabled = enabled;
        emit countEventEnabled(m_countEventEnabled);
    }
}

void DeviceController::setStartStopEventEnabled(bool enabled)
{
    if (m_startStopEventEnabled != enabled)
    {
        m_startStopEventEnabled = enabled;
        emit startStopEventEnabled(m_startStopEventEnabled);
    }
}
