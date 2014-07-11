#ifndef STOPWATCH_HOLDER_H
#define STOPWATCH_HOLDER_H

#include "stopwatch.h"

// A little utility classes that will update stopwatches 
// in functions that might return in the middle of evaluation
class StopwatchHolder
{
public:
  StopwatchHolder(Stopwatch & stopwatch);
  ~StopwatchHolder();

private:
	Stopwatch & mStopwatch;
};

#endif // #ifndef STOPWATCH_HOLDER_H
