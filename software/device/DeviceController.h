#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include <QColor>
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

    bool isCountEventEnabled() const;

    Event countEvent() const;
    void setCountEvent(Event event);

    bool isStartStopEventEnabled() const;

    Event startEvent() const;
    void setStartEvent(Event event);

    Event stopEvent() const;
    void setStopEvent(Event event);

    int duration() const;
    void setDuration(int duration);

    QColor color() const;
    void setColor(const QColor &color);

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

    void colorChanged(const QColor &color);

    void durationChanged(int duration);

    void controlInvalidated();

private:
    Trigger m_trigger;
    Mode m_mode;
    Event m_countEvent;
    Event m_startEvent;
    Event m_stopEvent;
    int m_duration;
    QColor m_color;
};

#endif // DEVICECONTROLLER_H
