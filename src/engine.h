#pragma once

#include "backend_GLSL.h"
#include "render_stage.h"
#include "texture.h"
#include "turtle_baseclass.h"
#include "uniform.h"
#include "window.h"

class Engine : TurtleBase
{
private:
   class MainCanvasStage : public turtle::RenderStage
   {
   public:
      explicit MainCanvasStage(std::string folder = "data");

      void update() override;
      void render(GLFWwindow *window) override;

      void next_texture();
      void previous_texture();
      turtle::ScaleVertices &zoom();

   private:
      turtle::TextureArray _textures;
      turtle::ScaleVertices _zoom;
   };

   class StaticCanvasStage : public turtle::RenderStage
   {
   public:
      StaticCanvasStage(const std::string &path, float x, float y, float width, float height,
                        float depth);

      void update() override;
      void render(GLFWwindow *window) override;

      turtle::Texture &texture();

   protected:
      turtle::Texture _texture;
   };

   class FrontCanvasStage : public StaticCanvasStage
   {
   public:
      FrontCanvasStage();
   };

   class LowerCanvasStage : public StaticCanvasStage
   {
   public:
      LowerCanvasStage();
   };

public:
   Engine(int argc, char *argv[]);
   ~Engine() = default;

   void run();


private:
   GLSL _backend;
   Window _win;
   MainCanvasStage _main_stage;
   FrontCanvasStage _front_stage;
   LowerCanvasStage _lower_stage;
   turtle::StageManager _stage_manager;

   static void key_call_back(GLFWwindow *window, int key, int seconds, int action, int mods);
};
