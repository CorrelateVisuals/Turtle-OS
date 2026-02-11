#pragma once

#include <GLFW/glfw3.h>

#include <vector>

namespace turtle
{

class RenderStage
{
public:
   virtual ~RenderStage() = default;
   virtual void update() = 0;
   virtual void render(GLFWwindow *window) = 0;
};

class StageManager
{
public:
   void add_stage(RenderStage &stage);
   void update();
   void render(GLFWwindow *window);

private:
   std::vector<RenderStage *> _stages;
};

} // namespace turtle
