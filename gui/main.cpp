#include "mainwindow.h"
#include "application.h"

int main(int argc, char *argv[])
{
  Application application(argc, argv);

  MainWindow mainWindow(application);
  mainWindow.setWindowTitle("Simple Chess");
  mainWindow.show();
  return application.exec();
}
