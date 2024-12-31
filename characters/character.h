#ifndef CHARACTER_H
#define CHARACTER_H

#include "../skeletal/skeletal-animation.h"
#include "../shader.h"
#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../scene.h"
#include "../drawable.h"

class Character : public Drawable
{
public:
  Character(const char * path) : mAnimation(path), mCurrentAnimation(0) { };
  virtual ~Character() { };

  bool setAnimation(unsigned int animationIndex);

  virtual void handleInput(const GLFWwindow* window, const Scene& scene) { };
  virtual void update(const float& time) override;
  virtual void draw(const Shader& shader) const override;
  virtual void free() const override;

protected:
  SkeletalAnimation mAnimation;
  const Animation* mCurrentAnimation;
  float mStartTime;
};

#endif
