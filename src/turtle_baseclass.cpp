#include "turtle_baseclass.h"

#include <iostream>
#include <string>

TurtleBase::DebugLevel TurtleBase::DEBUG_LEVEL = TurtleBase::DebugLevel::BASIC;

TurtleBase::TurtleBase(int argc, char *argv[])
{
   for (int i = 1; i < argc; ++i) {
      std::string argument = argv[i];
      if (argument == "turtle" && DEBUG_LEVEL != DebugLevel::OFF) {
         std::cout << "TurtleBase found!\n" << std::endl;
         break;
      }
   }
}
