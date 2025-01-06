#include "character.h"

bool Character::setAnimation(unsigned int animationIndex)
{
  const Animation* animation = mAnimation.setAnimation(animationIndex);
  if (animation != 0) {
    mCurrentAnimation = animation;
    mStartTime = glfwGetTime();
    return true;
  }

  return false;
}

void Character::update(const float& time) { mAnimation.updatePose(time); }

void Character::render(const Shader& shader) const
{
  shader.setMat4fv("u_model", model());
  shader.setMat3fv("u_trans_inver_model", glm::mat3(glm::transpose(glm::inverse(model()))));

  const Array<glm::mat4> transforms = mAnimation.pose();

  for (unsigned int i = 0; i < transforms.size(); i++) {
    shader.setMat4fv(("u_bone_transforms[" + std::to_string(i) + "]").c_str(), transforms[i]);
  }

  mAnimation.draw(shader);
};
