#include "DeviceChannel.h"

int DeviceChannel::min()
{
    return 0;
}

int DeviceChannel::max()
{
    return 255;
}

DeviceChannel::DeviceChannel(Coupling coupling, int threshold)
    : coupling(coupling),
      threshold(threshold)
{
}

qreal DeviceChannel::voltage() const
{
    return 10.0 * (threshold - 128) / 256;
}
