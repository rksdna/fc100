#include "DeviceSample.h"

DeviceSample::DeviceSample()
    : counter(0),
      timer(0),
      valid(false)
{
}

DeviceSample::DeviceSample(int counter, qreal timer)
    : counter(counter),
      timer(timer),
      valid(true)
{
}
