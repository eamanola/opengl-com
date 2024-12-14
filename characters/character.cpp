#include "character.h"
#include <glm/glm.hpp>

Character::Character(const char* path)
{
  mModel.loadModel(path);
}

Character::~Character()
{
}

void Character::setModelMatrix(glm::mat4 &modelMatrix)
{
  mModelMatrix = modelMatrix;
}

bool Character::setAnimation(unsigned int animationIndex)
{
  return mModel.setAnimation(animationIndex);
}

void Character::setSpeed(unsigned int speed)
{
  mModel.setSpeed(speed);
}

void Character::update(float time)
{
  mModel.update(time);
}

void Character::draw(Shader &shader)
{
  shader.use();

  const std::vector<glm::mat4> transforms = mModel.pose();
  for(unsigned int i = 0; i < transforms.size(); i++)
  {
    shader.setMat4fv("u_bone_transforms[" + std::to_string(i) + "]", transforms[i]);
  }
  shader.setMat4fv("u_model", mModelMatrix);

  mModel.draw(shader);
}

void Character::free()
{
  mModel.free();
}
