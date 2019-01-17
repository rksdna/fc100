#ifndef DEVICEMODE_H
#define DEVICEMODE_H

struct DeviceMode
{
    enum Edge
    {
        Ch1RisingEvent,
        Ch1FallingEvent,
        Ch2RisingEvent,
        Ch2FallingEvent
    };

    enum Clock
    {
        InternalClock,
        ExternalClock
    };

    DeviceMode(Edge startEdge = Ch1RisingEvent, Edge stopEdge = Ch1RisingEvent, Edge counterEgde = Ch1RisingEvent, Clock timerClock = InternalClock, int duration = 100);

    Edge startEdge;
    Edge stopEdge;
    Edge counterEgde;
    Clock timerClock;
    int duration;
};

#endif // DEVICEMODE_H
