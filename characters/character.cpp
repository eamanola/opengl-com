#include "character.h"
#include <glm/glm.hpp>

Character::Character(const char* path)
: mCurrentAnimation(0)
{
  loadModel(path);
}

bool Character::setAnimation(unsigned int animationIndex)
{
  const Animation* animation = SkeletalModel::setAnimation(animationIndex);
  if(animation != 0)
  {
    mCurrentAnimation = animation;
    mStartTime = glfwGetTime();
    return true;
  }

  return false;
}
