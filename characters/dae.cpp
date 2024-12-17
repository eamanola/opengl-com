#include "dae.h"
#include <glm/glm.hpp>
#include "../shader-utils.h"

Dae::Dae() : Character("assets/dae-runner/model.dae")
{
  glm::mat4 model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.f, -1.f, 0.f));
  model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  model = glm::scale(model, glm::vec3(0.25f));
  setModelMatrix(model);
}

Dae::~Dae()
{
}

void Dae::free()
{
  Character::free();

  glDeleteTextures(1, &textureId);
}
