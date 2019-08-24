#ifndef DEVICE_H
#define DEVICE_H

#include <QQueue>
#include <QTimer>

class QTimer;
class QJSEngine;
class QSettings;

class Device : public QObject
{
    Q_OBJECT

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

    enum Type
    {
        Number,
        Time,
        Frequency,
    };

    Q_ENUM(Type)

    enum FrequencyUnit
    {
        Hetz,
        KiloHertz,
        MegaHertz,
        GigaHertz
    };

    Q_ENUM(FrequencyUnit)

    enum TimeUnit
    {
        Second,
        MilliSecond,
        MicroSecond,
        NanoSecond
    };

    Q_ENUM(TimeUnit)

public:
    static Device *createDevice(const QString &type, QObject *parent = 0);

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

    qreal reference() const;
    void setReference(qreal reference);

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

    int maxSamplesCount() const;
    void setMaxSamplesCount(int count);

    TimeUnit timeUnit() const;
    void setTimeUnit(TimeUnit unit);

    int timeDecimals() const;
    void setTimeDecimals(int decimals);

    FrequencyUnit frequencyUnit() const;
    void setFrequencyUnit(FrequencyUnit unit);

    int frequencyDecimals() const;
    void setFrequencyDecimals(int decimals);

    QString function() const;
    void setFunction(const QString &function);

    QString functionUnit() const;
    void setFunctionUnit(const QString &unit);

    int functionDecimals() const;
    void setFunctionDecimals(int precision);

    bool isFunctionEnabled() const;
    void setFunctionEnabled(bool enabled);

    QString portName() const;
    void setPortName(const QString &name);

    qreal sample() const;
    qreal origin() const;
    qreal min() const;
    qreal max() const;

    QList<qreal> samples() const;

    void saveToSettings(QSettings &settings);
    void restoreFromSettings(QSettings &settings);

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

    void maxSamplesCountChanged(int count);
    void timeUnitChanged(TimeUnit unit);
    void timeDecimalsChanged(int decimals);
    void frequencyUnitChanged(FrequencyUnit unit);
    void frequencyDecimalsChanged(int decimals);
    void functionChanged(const QString &function);
    void functionUnitChanged(const QString &unit);
    void functionDecimalsChanged(int decimals);
    void functionEnabledChanged(bool enabled);
    void portNameChanged(const QString &name);

    void samplesChanged(const QList<qreal> &samples);
    void cleared();
    void sampleArr(qreal s);

protected:
    virtual void measure() = 0;
    void complete(qreal sample);

private:
    void timeout();

    void clearThenRestart();
    void setCountEventEnabled(bool enabled);
    void setStartStopEventEnabled(bool enabled);

    qreal function1(qreal sample);
    QString description(int threshold, Probe probe) const;

private:
    QTimer * const m_timer;
    QJSEngine * const m_engine;
    Coupling m_ch1Coupling;
    Probe m_ch1Probe;
    int m_ch1Threshold;
    Coupling m_ch2Coupling;
    Probe m_ch2Probe;
    int m_ch2Threshold;
    Clock m_clock;
    qreal m_reference;
    Trigger m_trigger;
    Mode m_mode;
    bool m_countEventEnabled;
    Event m_countEvent;
    bool m_startStopEventEnabled;
    Event m_startEvent;
    Event m_stopEvent;
    int m_duration;
    int m_maxSamplesCount;
    TimeUnit m_timeUnit;
    int m_timeDecimals;
    FrequencyUnit m_frequencyUnit;
    int m_frequencyDecimals;
    QString m_function;
    QString m_functionUnit;
    int m_functionDecimals;
    bool m_functionEnabled;
    QString m_portName;

    bool m_measure;
    bool m_delay;
    qreal m_sample;
    qreal m_origin;
    qreal m_min;
    qreal m_max;
    qreal m_smooth;

    QQueue<qreal> m_samples;

};

#endif // DEVICE_H
