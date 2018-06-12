#ifndef DEVICE_H
#define DEVICE_H

#include <QString>



struct DeviceInfo
{

};

class Device
{
public:
    enum Coupling
    {
        DcCompling,
        AcCoupling
    };

    enum Edge
    {
        Ch1RisingEdge,
        Ch1FallingEdge,
        Ch2RisingEdge,
        Ch2FallingEdge,
    };

    enum Clock
    {
        InternalClock,
        ExternalClock
    };

public:
    Device();




private:
    /*struct Input
    {
        Input();

        quint8 threshold;
        Coupling coupling;
    };

    struct Trigger
    {
        Trigger();
        Edge edge;

        quint8 vernier;
        quint8 minVernier;
        quint8 maxVernier;
    };

    struct Counter
    {
        Counter();

        quint32 value;
    };

    struct Timer
    {
        Timer();

        quint32 value;
    };*/



};

#endif // DEVICE_H
