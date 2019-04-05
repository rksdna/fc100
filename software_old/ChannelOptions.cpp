#include "ChannelOptions.h"

int ChannelOptions::min()
{
    return 0;
}

int ChannelOptions::max()
{
    return 255;
}

ChannelOptions::ChannelOptions(Coupling coupling, int threshold)
    : coupling(coupling),
      threshold(threshold)
{
}

qreal ChannelOptions::voltage() const
{
    return 10.0 * (threshold - 128) / 256;
}
