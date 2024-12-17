#ifndef CHARACTER_H
#define CHARACTER_H

#include "../skeletal/skeletal-model.h"
#include "../shader.h"
#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "../camera.h"

class Character
{
public:
  Character(const char * path);
  virtual ~Character();

  void setModelMatrix(const glm::mat4 &modelMatrix);
  bool setAnimation(unsigned int animationIndex);

  virtual void handleInput(const GLFWwindow* window, const Camera &camera);
  virtual void update(float time);
  virtual void draw(Shader &shader, const glm::mat4 &transform);
  virtual void free();

protected:
  float mStartTime;
  const Animation* mCurrentAnimation;
  SkeletalModel mModel;

private:
  glm::mat4 mModelMatrix;
};

#endif
