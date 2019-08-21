#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

class Device : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Coupling ch1Coupling READ ch1Coupling WRITE setCh1Coupling NOTIFY ch1CouplingChanged)
    Q_PROPERTY(Coupling ch2Coupling READ ch2Coupling WRITE setCh2Coupling NOTIFY ch2CouplingChanged)
    Q_PROPERTY(Probe ch1Probe READ ch1Probe WRITE setCh1Probe NOTIFY ch1ProbeChanged)
    Q_PROPERTY(Probe ch2Probe READ ch2Probe WRITE setCh2Probe NOTIFY ch2ProbeChanged)
    Q_PROPERTY(int ch1Threshold READ ch1Threshold WRITE setCh1Threshold NOTIFY ch1ThresholdChanged)
    Q_PROPERTY(int ch2Threshold READ ch2Threshold WRITE setCh2Threshold NOTIFY ch2ThresholdChanged)

    Q_PROPERTY(Clock clock READ clock WRITE setClock NOTIFY clockChanged)

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
        FrequencyMode,
        PeriodMode,
        CountMode,
        TimeMode,
        DutyMode
    };

    Q_ENUM(Mode)

    enum Event
    {
        Ch1RisingEdgeEvent,
        Ch1FallingEdgeEvent,
        Ch2RisingEdgeEvent,
        Ch2FallingEdgeEvent,
        AutoSelectedEvent
    };

    Q_ENUM(Event)

public:
    explicit Device(QObject *parent = 0);

    Coupling ch1Coupling() const;
    void setCh1Coupling(Coupling coupling);

    Coupling ch2Coupling() const;
    void setCh2Coupling(Coupling coupling);

    Probe ch1Probe() const;
    void setCh1Probe(Probe probe);

    Probe ch2Probe() const;
    void setCh2Probe(Probe probe);

    int ch1Threshold() const;
    void setCh1Threshold(int threshold);

    int ch2Threshold() const;
    void setCh2Threshold(int threshold);

    Clock clock() const;
    void setClock(Clock clock);

signals:
    void ch1CouplingChanged(Coupling coupling);
    void ch2CouplingChanged(Coupling coupling);

    void ch1ProbeChanged(Probe probe);
    void ch2ProbeChanged(Probe probe);

    void ch1ThresholdChanged(int threshold);
    void ch2ThresholdChanged(int threshold);

    void clockChanged(Clock clock);

private:
    Coupling m_ch1Coupling;
    Coupling m_ch2Coupling;
    Probe m_ch1Probe;
    Probe m_ch2Probe;
    int m_ch1Threshold;
    int m_ch2Threshold;


    Clock m_clock;
    double m_clockFrequency;

    Mode m_mode;
    Event m_countEvent;
    Event m_startEvent;
    Event m_stopEvent;
    int m_duration;
};

#endif // DEVICE_H
