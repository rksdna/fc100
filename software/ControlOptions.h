#ifndef CONTROLOPTIONS_H
#define CONTROLOPTIONS_H

struct ControlOptions
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

    ControlOptions(bool burst = false, Edge startEdge = Ch1RisingEvent, Edge stopEdge = Ch1RisingEvent, Edge counterEgde = Ch1RisingEvent, Clock timerClock = InternalClock, int duration = 100);

    bool burst;
    Edge startEdge;
    Edge stopEdge;
    Edge counterEgde;
    Clock timerClock;
    int duration;
};

#endif // CONTROLOPTIONS_H
