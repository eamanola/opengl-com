#ifndef SCENE_H
#define SCENE_H

#define GLFW_INCLUDE_NONE
#include "camera.h"
#include <GLFW/glfw3.h>

class Scene
{
public:
  Scene() { }
  virtual ~Scene() { }

  virtual void setup() { }
  virtual void update(const float& time) { }
  virtual void render(const Camera& camera) const { }
  virtual void render() const { render(camera()); }
  virtual void teardown() { }

  virtual void handleInput(const GLFWwindow* window) { }
  virtual void onChar(const char c) { }
  virtual void onMouse(const GLFWwindow* window, const double x, const double y) { }
  virtual void onScroll(const GLFWwindow* window, const double x, const double y) { }

  virtual const Camera& camera() const { return mCamera; }

protected:
  Camera mCamera;
};

#endif
