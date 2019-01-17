#include "DeviceMode.h"

DeviceMode::DeviceMode(DeviceMode::Event startEvent, DeviceMode::Event stopEvent, DeviceMode::Event counterEvent, DeviceMode::Clock timerClock, int duration)
    : startEvent(startEvent),
      stopEvent(stopEvent),
      counterEvent(counterEvent),
      timerClock(timerClock),
      duration(duration)
{
}
