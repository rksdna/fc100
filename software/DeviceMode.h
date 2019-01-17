#ifndef DEVICEMODE_H
#define DEVICEMODE_H

struct DeviceMode
{
    enum Event
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

    DeviceMode(Event startEvent = Ch1RisingEvent, Event stopEvent = Ch1RisingEvent, Event counterEvent = Ch1RisingEvent, Clock timerClock = InternalClock, int duration = 100);

    Event startEvent;
    Event stopEvent;
    Event counterEvent;
    Clock timerClock;
    int duration;
};

#endif // DEVICEMODE_H
