#ifndef DEVICEMODE_H
#define DEVICEMODE_H

class DeviceMode
{
public:
    enum Edge
    {
        Ch1RisingEdge,
        Ch1FallingEdge,
        Ch2RisingEdge,
        Ch2FallingEdge
    };

    enum Clock
    {
        InternalClock,
        ExternalClock
    };

public:
    DeviceMode();

    Edge startEdge() const;
    void setStartEdge(Edge value);

    Edge stopEdge() const;
    void setStopEdge(Edge value);

    Edge counterEdge() const;
    void setCounterEdge(Edge value);

    Clock timerClock() const;
    void setTimerClock(Clock value);

    int duration() const;
    void setDuration(int value);

private:
    Edge m_startEdge;
    Edge m_stopEdge;
    Edge m_counterEdge;
    Clock m_timerClock;
    int m_duration;
};

#endif // DEVICEMODE_H
