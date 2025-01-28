#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class Scene
{
public:
  Scene() { }
  virtual ~Scene() { }

  virtual void setup() { }
  virtual void update(const float& time) { }
  virtual void render(const unsigned int fbo, const Camera& camera) const { }
  virtual void render() const { render(0, camera()); }
  virtual void
  renderShadowMap(const unsigned int fbo, const glm::mat4& projection, const glm::mat4& view) const
  {
  }
  virtual void renderCubeMap(
    const unsigned int fbo,
    const std::vector<glm::mat4>& shadowMatrices,
    const glm::vec3& lightPos,
    const float& far
  ) const
  {
  }
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
