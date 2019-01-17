#include "DeviceMode.h"

DeviceMode::DeviceMode(DeviceMode::Edge startEvent, DeviceMode::Edge stopEvent, DeviceMode::Edge counterEvent, DeviceMode::Clock timerClock, int duration)
    : startEdge(startEvent),
      stopEdge(stopEvent),
      counterEgde(counterEvent),
      timerClock(timerClock),
      duration(duration)
{
}
