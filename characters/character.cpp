#include "character.h"
#include <glm/glm.hpp>

Character::Character(const char* path)
: mCurrentAnimation(0)
{
  mModel.loadModel(path);
}

Character::~Character()
{
}

void Character::setModelMatrix(const glm::mat4 &modelMatrix)
{
  mModelMatrix = modelMatrix;
}

bool Character::setAnimation(unsigned int animationIndex)
{
  const Animation* animation = mModel.setAnimation(animationIndex);
  if(animation != 0)
  {
    mCurrentAnimation = animation;
    mStartTime = glfwGetTime();
    return true;
  }

  return false;
}

void Character::update(float time)
{
  mModel.update(time);
}

void Character::draw(Shader &shader, const glm::mat4 &transform)
{
  shader.use();

  const std::vector<glm::mat4> transforms = mModel.pose();
  for(unsigned int i = 0; i < transforms.size(); i++)
  {
    shader.setMat4fv("u_bone_transforms[" + std::to_string(i) + "]", transforms[i]);
  }
  glm::mat4 model = transform * mModelMatrix;
  shader.setMat4fv("u_model", model);

  mModel.draw(shader);
}

void Character::handleInput(const GLFWwindow* window, const Camera &camera)
{

}

void Character::free()
{
  mModel.free();
}
