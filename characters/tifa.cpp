#include "tifa.h"
#include <glm/glm.hpp>

Tifa::Tifa()
:
Character("assets/tifa/dancing/source/Bellydancing.fbx")
// Character("assets/tifa/fighting/source/Tifa 4_1.glb")
{
  glm::mat4 model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.f, -1.f, 0.f));
  setModelMatrix(model);
}

Tifa::~Tifa()
{
}
