#include "whipper.h"
#include <glm/glm.hpp>
#include "../shader-utils.h"

Whipper::Whipper() : Character("assets/whipper/scene.gltf")
{
  glm::mat4 model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(1.5f, -1.f, 1.f));
  model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  model = glm::scale(model, glm::vec3(0.15f));
  setModelMatrix(model);

  setAnimation(11);

  textureId = ShaderUtils::loadTexture("assets/pink.png");
}

Whipper::~Whipper()
{
}

void Whipper::draw(Shader &shader)
{
  shader.use();
  glActiveTexture(GL_TEXTURE0);
  shader.setInt("u_material.texture_diffuse1", 0);
  glBindTexture(GL_TEXTURE_2D, textureId);

  Character::draw(shader);

  glActiveTexture(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Whipper::free()
{
  Character::free();

  glDeleteTextures(1, &textureId);
}
