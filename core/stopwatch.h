#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <ctime>
#include <iostream>
#include <chrono>

class Stopwatch
{
public:
	Stopwatch(bool startImmediately = false);
	~Stopwatch(void);

  void reset();
  void start();
  void stop();
  double timeElapsed();
  std::string timeElapsedString(int precision = 3);

private:
  bool mRunning;
  std::chrono::time_point<std::chrono::system_clock> mStartTime;
  std::chrono::time_point<std::chrono::system_clock> mEndTime;
};

#endif // #ifndef STOPWATCH_H
