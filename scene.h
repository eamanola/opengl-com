#ifndef SCENE_H
#define SCENE_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Scene
{
public:
  Scene();
  virtual ~Scene();

  virtual void setup() = 0;
  virtual void update(const float &time) = 0;
  virtual void render() = 0;
  virtual void teardown() = 0;

  virtual void handleInput(const GLFWwindow* window) = 0;
  virtual void onChar(const char c) = 0;
  virtual void onMouse(const GLFWwindow* window, const double x, const double y) = 0;
  virtual void onScroll(const GLFWwindow* window, const double x, const double y) = 0;
};

#endif
