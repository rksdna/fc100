#ifndef DEVICECHANNEL_H
#define DEVICECHANNEL_H

#include <QtGlobal>

struct DeviceChannel
{
    enum Probe
    {
        x1Probe,
        x10Probe,
        x100Probe
    };

    enum Coupling
    {
        DcCoupling,
        AcCoupling
    };

    static int min();
    static int max();

    DeviceChannel(Coupling coupling = DcCoupling, Probe probe = x1Probe, int threshold = 128);

    int decimals() const;
    qreal voltage() const;

    Coupling coupling;
    Probe probe;
    int threshold;
};

#endif // DEVICECHANNEL_H
