#ifndef DEVICE_H
#define DEVICE_H

#include <QQueue>
#include <QTimer>

class QTimer;
class QJSEngine;
class QSettings;

class DeviceChannel;
class DeviceReference;
class DeviceProcessor;

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

    DeviceChannel *channel1() const;
    DeviceChannel *channel2() const;
    DeviceReference *reference() const;
    DeviceProcessor *processor() const;

    void tmp();
    void restart();
    void clear();

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

private:
    QTimer * const m_timer;
    QJSEngine * const m_engine;

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
