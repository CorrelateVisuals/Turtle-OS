#include "render_stage.h"

namespace turtle
{

void StageManager::add_stage(RenderStage &stage)
{
   _stages.push_back(&stage);
}

void StageManager::update()
{
   for (RenderStage *stage : _stages) {
      stage->update();
   }
}

void StageManager::render(GLFWwindow *window)
{
   for (RenderStage *stage : _stages) {
      stage->render(window);
   }
}

} // namespace turtle
