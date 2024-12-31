#include "character.h"
#include <glm/glm.hpp>

bool Character::setAnimation(unsigned int animationIndex)
{
  const Animation* animation = mAnimation.setAnimation(animationIndex);
  if(animation != 0)
  {
    mCurrentAnimation = animation;
    mStartTime = glfwGetTime();
    return true;
  }

  return false;
}

void Character::update(const float& time)
{
  mAnimation.updatePose(time);
}

void Character::draw(const Shader& shader) const
{
  shader.setMat4fv("u_model", model());

  const Array<glm::mat4> transforms = mAnimation.pose();

  for(unsigned int i = 0; i < transforms.size(); i++)
  {
    shader.setMat4fv(("u_bone_transforms[" + std::to_string(i) + "]").c_str(), transforms[i]);
  }

  mAnimation.draw(shader);
};


