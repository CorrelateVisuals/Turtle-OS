#pragma once

#include "geometry.h"
#include "texture.h"
#include "uniform.h"

class TurtleBase
{
public:
   enum class DebugLevel { OFF = 0, BASIC = 1, DETAILED = 2 };

   static DebugLevel DEBUG_LEVEL;

   TurtleBase(int argc, char *argv[]);
   virtual ~TurtleBase() = default;
};