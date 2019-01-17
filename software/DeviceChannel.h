#ifndef DEVICECHANNEL_H
#define DEVICECHANNEL_H

#include <QtGlobal>

struct DeviceChannel
{
    enum Coupling
    {
        DcCoupling,
        AcCoupling
    };

    static int min();
    static int max();

    DeviceChannel(Coupling coupling = DcCoupling, int threshold = 128);

    qreal voltage() const;

    Coupling coupling;
    int threshold;
};

#endif // DEVICECHANNEL_H
