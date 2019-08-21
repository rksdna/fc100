#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

class Device : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Coupling ch1Coupling READ ch1Coupling WRITE setCh1Coupling NOTIFY ch1CouplingChanged)
    Q_PROPERTY(Coupling ch2Coupling READ ch2Coupling WRITE setCh2Coupling NOTIFY ch2CouplingChanged)
    Q_PROPERTY(Clock clock READ clock WRITE setClock NOTIFY clockChanged)

public:
    enum Coupling
    {
        DcCoupling,
        AcCoupling
    };

    Q_ENUM(Coupling)

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

    Clock clock() const;
    void setClock(Clock clock);

signals:
    void ch1CouplingChanged(Coupling coupling);
    void ch2CouplingChanged(Coupling coupling);
    void clockChanged(Clock clock);

private:
    Coupling m_ch1Coupling;
    Coupling m_ch2Coupling;

    Clock m_clock;
    double m_clockFrequency;

    Mode m_mode;
    Event m_countEvent;
    Event m_startEvent;
    Event m_stopEvent;
    int m_duration;
};

#endif // DEVICE_H
