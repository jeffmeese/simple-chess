#include "application.h"
#include "consolegame.h"

Application::Application(int & argc, char ** argv)
  : QCoreApplication(argc, argv)
  , mGame(new ConsoleGame)
  , mArgc(argc)
  , mArgv(argv)
{
}

Application::~Application()
{

}

int Application::execute()
{
  if (!readConfigFile("sc.config"))
    return -1;

  if (mArgc > 1) {
    std::string command = mArgv[1];
    if (command == "--perft") {
      int level = atoi(mArgv[2]);
      mGame->doPerft(level);
      return 0;
    }
  }

  return mGame->run();
}

bool Application::readConfigFile(const std::string &fileName)
{
  // This will eventually be where we read configuration settings
  return true;
}
