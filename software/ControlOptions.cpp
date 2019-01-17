#include "ControlOptions.h"

ControlOptions::ControlOptions(bool burst, ControlOptions::Edge startEvent, ControlOptions::Edge stopEvent, ControlOptions::Edge counterEvent, ControlOptions::Clock timerClock, int duration)
    : burst(burst),
      startEdge(startEvent),
      stopEdge(stopEvent),
      counterEgde(counterEvent),
      timerClock(timerClock),
      duration(duration)
{
}
