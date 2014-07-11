#include "application.h"
#include "consolegame.h"

Application::Application(int & argc, char ** argv)
  : QCoreApplication(argc, argv)
  , mGame(new ConsoleGame)
{
}

Application::~Application()
{

}

int Application::execute()
{
  if (!readConfigFile("sc.config"))
    return -1;

  return mGame->run();
}

bool Application::readConfigFile(const std::string &fileName)
{
  // This will eventually be where we read configuration settings
  return true;
}
