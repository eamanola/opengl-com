#ifndef CHARACTER_H
#define CHARACTER_H

#include "../skeletal/skeletal-model.h"
#include "../shader.h"
#include <glm/glm.hpp>

class Character
{
public:
  Character(const char * path);
  virtual ~Character();

  void setModelMatrix(glm::mat4 &modelMatrix);
  bool setAnimation(unsigned int animationIndex);
  void setSpeed(unsigned int speed);
  void update(float time);
  virtual void draw(Shader &shader);
  virtual void free();

private:
  SkeletalModel mModel;
  glm::mat4 mModelMatrix;
};

#endif
