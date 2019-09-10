#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

class QTimer;
class QSettings;

class DeviceChannel;
class DeviceProcessor;
class DeviceReference;
class DeviceController;

class Device : public QObject
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
    static Device *createDevice(const QString &type, QObject *parent = 0);

public:
    explicit Device(QObject *parent = 0);

    DeviceChannel *channel1() const;
    DeviceChannel *channel2() const;
    DeviceReference *reference() const;
    DeviceProcessor *processor() const;

    void restart();
    void clear();
    void stop();

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

    QString portName() const;
    void setPortName(const QString &name);

    void saveToSettings(QSettings &settings) const;
    void restoreFromSettings(QSettings &settings);

signals:
    void triggerChanged(Trigger trigger);
    void modeChanged(Mode mode);
    void countEventEnabled(bool enabled);
    void startStopEventEnabled(bool enabled);
    void countEventChanged(Event event);
    void startEventChanged(Event event);
    void stopEventChanged(Event event);
    void durationChanged(int duration);

protected:
    virtual void measure() = 0;
    void complete(qreal sample);

private:
    void timeout();
    void clearThenRestart();
    void setCountEventEnabled(bool enabled);
    void setStartStopEventEnabled(bool enabled);

private:
    QTimer * const m_timer;

    DeviceChannel * const m_channel1;
    DeviceChannel * const m_channel2;
    DeviceReference * const m_reference;
    DeviceProcessor * const m_processor;

    Trigger m_trigger;
    Mode m_mode;
    bool m_countEventEnabled;
    Event m_countEvent;
    bool m_startStopEventEnabled;
    Event m_startEvent;
    Event m_stopEvent;
    int m_duration;

    QString m_portName;

    bool m_measure;
    bool m_delay;
};

#endif // DEVICE_H
