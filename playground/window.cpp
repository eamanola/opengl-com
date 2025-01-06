#include "window.h"

#include "shaders/u-material.h"
#include "shapes.h"
#include "utils/utils.h"
#include <glm/gtc/matrix_transform.hpp>

Window::Window() :
  mMesh(Shapes::QUAD),
  mTexture(Utils::loadTexture2D("assets/blending_transparent_window.png", TEXTURE_TYPE_DIFFUSE))
{
}

void Window::render(const Shader& shader) const
{
  glDisable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  UMaterial::setTextures(shader, &mTexture, 1);

  glm::mat4 model = this->model();

  model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.55f));
  shader.setMat4fv("u_model", model);
  shader.setMat3fv("u_trans_inver_model", glm::mat3(glm::transpose(glm::inverse(model))));
  mMesh.draw();

  // order matters / depth buffer
  // 2nd will glitch through last window
  // semi transpaers shoudl draw last, and in order from furthest to nearest
  model = glm::translate(glm::mat4(1.0f), glm::vec3(0.4f, 1.2f, 0.6f));
  shader.setMat4fv("u_model", model);
  shader.setMat3fv("u_trans_inver_model", glm::mat3(glm::transpose(glm::inverse(model))));
  mMesh.draw();

  model = glm::translate(glm::mat4(1.0f), glm::vec3(0.6f, 0.8f, 0.555f));
  shader.setMat4fv("u_model", model);
  shader.setMat3fv("u_trans_inver_model", glm::mat3(glm::transpose(glm::inverse(model))));
  mMesh.draw();

  UMaterial::clearTextures(shader, &mTexture, 1);

  glDisable(GL_BLEND);

  glEnable(GL_CULL_FACE);
}

void Window::free() const
{
  mMesh.free();
  Utils::deleteTextures({ mTexture });
}
