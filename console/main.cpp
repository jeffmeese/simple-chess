#include "application.h"

#include <core/board0x88.h>
#include <iostream>

int main(int argc, char ** argv)
{
  Application application(argc, argv);
  return application.execute();
}
