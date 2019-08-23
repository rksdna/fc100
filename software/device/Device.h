#ifndef DEVICE_H
#define DEVICE_H

#include <QTimer>
#include <QObject>

class Device : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Coupling ch1Coupling READ ch1Coupling WRITE setCh1Coupling NOTIFY ch1CouplingChanged)
    Q_PROPERTY(Probe ch1Probe READ ch1Probe WRITE setCh1Probe NOTIFY ch1ProbeChanged)
    Q_PROPERTY(int ch1Threshold READ ch1Threshold WRITE setCh1Threshold NOTIFY ch1ThresholdChanged)
    Q_PROPERTY(Probe ch2Probe READ ch2Probe WRITE setCh2Probe NOTIFY ch2ProbeChanged)
    Q_PROPERTY(Coupling ch2Coupling READ ch2Coupling WRITE setCh2Coupling NOTIFY ch2CouplingChanged)
    Q_PROPERTY(int ch2Threshold READ ch2Threshold WRITE setCh2Threshold NOTIFY ch2ThresholdChanged)

    Q_PROPERTY(Clock clock READ clock WRITE setClock NOTIFY clockChanged)

public:
    static Device *createDevice(const QString &type, QObject *parent = 0);

public:
    enum Coupling
    {
        DcCoupling,
        AcCoupling
    };

    Q_ENUM(Coupling)

    enum Probe
    {
        x1Probe,
        x10Probe,
        x100Probe
    };

    Q_ENUM(Probe)

    enum Clock
    {
        InternalClock,
        ExternalClock
    };

    Q_ENUM(Clock)

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
    explicit Device(QObject *parent = 0);

    void reset();

    void restart();
    void clear();

    Coupling ch1Coupling() const;
    void setCh1Coupling(Coupling coupling);

    Probe ch1Probe() const;
    void setCh1Probe(Probe probe);

    int ch1Threshold() const;
    void setCh1Threshold(int threshold);

    Coupling ch2Coupling() const;
    void setCh2Coupling(Coupling coupling);

    Probe ch2Probe() const;
    void setCh2Probe(Probe probe);

    int ch2Threshold() const;
    void setCh2Threshold(int threshold);

    Clock clock() const;
    void setClock(Clock clock);

    qreal clockFrequency() const;
    void setClockFrequency(qreal frequency);

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

signals:
    void ch1CouplingChanged(Coupling coupling);
    void ch1ProbeChanged(Probe probe);
    void ch1ThresholdChanged(int threshold);
    void ch1DescriptionChanged(const QString &description);

    void ch2CouplingChanged(Coupling coupling);
    void ch2ProbeChanged(Probe probe);
    void ch2ThresholdChanged(int threshold);
    void ch2DescriptionChanged(const QString &description);

    void clockChanged(Clock clock);
    void clockFrequencyChanged(qreal frequency);

    void triggerChanged(Trigger trigger);
    void modeChanged(Mode mode);
    void countEventEnabled(bool enabled);
    void startStopEventEnabled(bool enabled);
    void countEventChanged(Event event);
    void startEventChanged(Event event);
    void stopEventChanged(Event event);
    void durationChanged(int duration);

protected:
    virtual void measure() {}
    void complete(bool valid, qreal value);

private:
    void timeout();

    void clearThenRestart();
    void setCountEventEnabled(bool enabled);
    void setStartStopEventEnabled(bool enabled);

    QString description(int threshold, Probe probe) const;

private:
    QTimer * const m_timer;
    Coupling m_ch1Coupling;
    Probe m_ch1Probe;
    int m_ch1Threshold;
    Coupling m_ch2Coupling;
    Probe m_ch2Probe;
    int m_ch2Threshold;
    Clock m_clock;
    qreal m_clockFrequency;
    Trigger m_trigger;
    Mode m_mode;
    bool m_countEventEnabled;
    Event m_countEvent;
    bool m_startStopEventEnabled;
    Event m_startEvent;
    Event m_stopEvent;
    int m_duration;
    bool m_measure;
    bool m_delay;
};

#endif // DEVICE_H
