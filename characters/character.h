#ifndef CHARACTER_H
#define CHARACTER_H

#include "renderable.h"

#include "scene.h"
#include "shader.h"
#include "skeletal/skeletal-animation.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Character : public Renderable
{
public:
  Character(const char* path) : mAnimation(path), mCurrentAnimation(0) { }
  virtual ~Character() { }

  bool setAnimation(unsigned int animationIndex);

  virtual void handleInput(const GLFWwindow* window, const Scene& scene) { }
  virtual void update(const float& time) override;
  virtual void render(const Shader& shader) const override;
  virtual void free() const override { mAnimation.free(); }

protected:
  SkeletalAnimation mAnimation;
  const Animation* mCurrentAnimation;
  float mStartTime;
};

#endif
