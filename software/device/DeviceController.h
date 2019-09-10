#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QObject>

class QSettings;

class DeviceController : public QObject
{
    Q_OBJECT

public:
    enum Mode
    {
        TimeMode,
        FrequencyMode,
        PeriodMode,
        CountMode,
        DutyMode,
        GatePeriodMode,
        GateFrequencyMode,
        GateCountMode,
    };

    Q_ENUM(Mode)

    enum Trigger
    {
        AutoTrigger,
        ManualTrigger,
    };

    Q_ENUM(Trigger)

    enum Event
    {
        Ch1RisingEdgeEvent,
        Ch1FallingEdgeEvent,
        Ch2RisingEdgeEvent,
        Ch2FallingEdgeEvent
    };

    Q_ENUM(Event)

public:
    explicit DeviceController(QObject *parent = 0);

    Trigger trigger() const;
    void setTrigger(Trigger trigger);

    Mode mode() const;
    void setMode(Mode mode);

    Event countEvent() const;
    void setCountEvent(Event event);

    Event startEvent() const;
    void setStartEvent(Event event);

    Event stopEvent() const;
    void setStopEvent(Event event);

    int duration() const;
    void setDuration(int duration);

    void saveToSettings(QSettings &settings) const;
    void restoreFromSettings(QSettings &settings);

signals:
    void triggerChanged(Trigger trigger);
    void modeChanged(Mode mode);

    void countEventEnabled(bool enabled);
    void countEventChanged(Event event);

    void startStopEventEnabled(bool enabled);
    void startEventChanged(Event event);
    void stopEventChanged(Event event);

    void durationChanged(int duration);

    void controlInvalidated();

private:
    void setCountEventEnabled(bool enabled);
    void setStartStopEventEnabled(bool enabled);

private:
    Trigger m_trigger;
    Mode m_mode;
    bool m_countEventEnabled;
    Event m_countEvent;
    bool m_startStopEventEnabled;
    Event m_startEvent;
    Event m_stopEvent;
    int m_duration;
};

#endif // DEVICECONTROLLER_H
