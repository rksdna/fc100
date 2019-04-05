#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QJsonObject>
#include "EnumControl.h"

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

    enum Edge
    {
        Ch1RisingEdge,
        Ch1FallingEdge,
        Ch2RisingEdge,
        Ch2FallingEdge
    };

    Q_ENUM(Edge)

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

        GateFrequencyMode,
        GatePeriodMode,
        Gate

    };

    Q_ENUM(Mode)

    enum Run
    {
        ManualRun,
        AutoRun
    };

    Q_ENUM(Run)

public:
    explicit Device(QObject *parent = 0);

    QJsonObject toJsonObject() const;
    void setJsonObject(const QJsonObject &object);

protected:
    EnumControl<Coupling> * const m_ch1Coupling;
    EnumControl<Coupling> * const m_ch2Coupling;
    EnumControl<Edge> * const m_startEdge;
    EnumControl<Edge> * const m_stopEdge;
    EnumControl<Edge> * const m_countEdge;
    EnumControl<Clock> * const m_clock;
    EnumControl<Mode> * const m_mode;
    EnumControl<Run> * const m_run;
};

#endif // DEVICE_H
