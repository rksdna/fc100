#ifndef DEVICESAMPLEFILTER_H
#define DEVICESAMPLEFILTER_H

#include <QQueue>
#include "DeviceSample.h"

class DeviceSampleFilter
{
public:
    enum Target
    {
        ActualValue,
        AverageValue
    };

public:
    DeviceSampleFilter();

    int size() const;
    void setSize(int size);

    void addSample(const DeviceSample &sample);

private:
    void dropExcessSamples();

private:
    int m_size;
    QQueue<DeviceSample> m_samples;
};

#endif // DEVICESAMPLEFILTER_H
