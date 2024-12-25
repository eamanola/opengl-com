#ifndef CHARACTER_H
#define CHARACTER_H

#include "../skeletal/skeletal-model.h"
#include "../shader.h"
#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../scene.h"

class Character : public SkeletalModel
{
public:
  Character(const char * path);
  virtual ~Character() { };

  bool setAnimation(unsigned int animationIndex);

  virtual void handleInput(const GLFWwindow* window, const Scene& scene) { };

protected:
  float mStartTime;
  const Animation* mCurrentAnimation;
};

#endif
