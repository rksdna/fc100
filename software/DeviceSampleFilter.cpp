#include "DeviceSampleFilter.h"

DeviceSampleFilter::DeviceSampleFilter()
    : m_size(10)
{
}

int DeviceSampleFilter::size() const
{
    return m_size;
}

void DeviceSampleFilter::setSize(int size)
{
    m_size = size;
    dropExcessSamples();
}

void DeviceSampleFilter::addSample(const DeviceSample &sample)
{
    m_samples.append(sample);
    dropExcessSamples();
}

void DeviceSampleFilter::dropExcessSamples()
{
    while (m_samples.size() > m_size)
        m_samples.dequeue();
}


