#include "DeviceChannel.h"

int DeviceChannel::min()
{
    return 0;
}

int DeviceChannel::max()
{
    return 255;
}

DeviceChannel::DeviceChannel(Coupling coupling, Probe probe, int threshold)
    : coupling(coupling),
      probe(probe),
      threshold(threshold)
{
}

int DeviceChannel::decimals() const
{
    switch (probe)
    {
    case x1Probe:
        return 2;
    case x10Probe:
        return 1;
    case x100Probe:
        return 0;
    }

    return 0;
}

qreal DeviceChannel::voltage() const
{
    const qreal voltage = 10.0 * (threshold - 127) / 256;
    switch (probe)
    {
    case x1Probe:
        return voltage;
    case x10Probe:
        return 10 * voltage;
    case x100Probe:
        return 100 * voltage;
    }

    return 0.0;
}
