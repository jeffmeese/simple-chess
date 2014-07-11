#ifndef APPLICATION_H
#define APPLICATION_H

#include <QCoreApplication>

#include <memory>

class ConsoleGame;

class Application
    : public QCoreApplication
{
  Q_OBJECT

public:
  explicit Application(int & argc, char ** argv);
  ~Application();

  int execute();

private:
  bool readConfigFile(const std::string & fileName);

private:
  std::unique_ptr<ConsoleGame> mGame;
};

#endif // APPLICATION_H
