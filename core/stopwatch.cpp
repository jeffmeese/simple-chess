#include "stopwatch.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

Stopwatch::Stopwatch(bool startImmediately)
  : mRunning(false)
  , mStartTime(std::chrono::system_clock::now())
  , mEndTime(std::chrono::system_clock::now())
{
	if (startImmediately)
    start();
}

Stopwatch::~Stopwatch(void)
{
  stop();
}

std::string Stopwatch::timeElapsedString(int precision)
{
  std::ostringstream oss;
  oss << std::setprecision(precision) << std::setiosflags(std::ios::fixed) << timeElapsed();
  return oss.str();
}

double Stopwatch::timeElapsed()
{
  // Get the time elapsed
  // If the timer is not running use the end time stored during stop()
  std::chrono::duration<double> timeElapsed = std::chrono::system_clock::now()-mStartTime;
  if (!mRunning)
    timeElapsed = mEndTime - mStartTime;

  return timeElapsed.count();
}

void Stopwatch::reset()
{
  mRunning = true;
  mStartTime = std::chrono::system_clock::now();
}

void Stopwatch::start()
{
  mRunning = true;
  mStartTime = std::chrono::system_clock::now();
}

void Stopwatch::stop()
{
  mRunning = false;
  mEndTime = std::chrono::system_clock::now();
}

