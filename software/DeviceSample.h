#ifndef DEVICESAMPLE_H
#define DEVICESAMPLE_H

#include <QtGlobal>

struct DeviceSample
{
    DeviceSample();
    DeviceSample(int counter, qreal timer);

    qreal counter;
    qreal timer;
    bool valid;
};

#endif // DEVICESAMPLE_H
