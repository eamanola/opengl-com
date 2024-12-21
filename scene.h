#ifndef SCENE_H
#define SCENE_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "camera.h"

class Scene
{
public:
  Scene() { };
  virtual ~Scene() { };

  virtual void setup() { };
  virtual void update(const float &time) { };
  virtual void render() { };
  virtual void teardown() { };

  virtual void handleInput(const GLFWwindow* window) { };
  virtual void onChar(const char c) { };
  virtual void onMouse(const GLFWwindow* window, const double x, const double y) { };
  virtual void onScroll(const GLFWwindow* window, const double x, const double y) { };

  virtual Camera& camera() { return mCamera; };
  virtual const glm::vec3& cameraDir() const { return mCamera.front(); };

private:
  Camera mCamera;
};

#endif
