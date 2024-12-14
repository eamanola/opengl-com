#include "tifa.h"
#include <glm/glm.hpp>

Tifa::Tifa() : Character("assets/tifa2/source/Bellydancing.fbx")
{
  glm::mat4 model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-1.5f, -1.f, 1.f));
  setModelMatrix(model);
  setSpeed(100);
}

Tifa::~Tifa()
{
}
