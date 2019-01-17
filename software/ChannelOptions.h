#ifndef CHANNELOPTIONS_H
#define CHANNELOPTIONS_H

#include <QtGlobal>

struct ChannelOptions
{
    enum Coupling
    {
        DcCoupling,
        AcCoupling
    };

    static int min();
    static int max();

    ChannelOptions(Coupling coupling = DcCoupling, int threshold = 128);

    qreal voltage() const;

    Coupling coupling;
    int threshold;
};

#endif // CHANNELOPTIONS_H
