#include "stopwatchholder.h"
#include "stopwatch.h"

StopwatchHolder::StopwatchHolder(Stopwatch & stopwatch)
  : mStopwatch(stopwatch)
{
  mStopwatch.start();
}

StopwatchHolder::~StopwatchHolder(void)
{
  mStopwatch.stop();
}
