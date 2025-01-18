#include "box.h"

#include "shaders/u-material.h"
#include "shaders/u-model.h"
#include "shapes.h"
#include "utils/utils.h"
#include <glm/gtc/matrix_transform.hpp>

Box::Box() :
  mMesh(Shapes::CUBE),
  mTextures({
    Utils::Textures::loadTexture2D("assets/container2.png", TEXTURE_TYPE_DIFFUSE),
    Utils::Textures::loadTexture2D("assets/container2_specular.png", TEXTURE_TYPE_SPECULAR),
  }),
  mPositions({
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f),
  })
{
  setModel(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1.f, -1.f)));
  setupBuffers();
}

const std::vector<glm::mat4> Box::models() const
{
  std::vector<glm::mat4> models;

  for (unsigned int i = 0; i < mPositions.size(); i++) {
    glm::mat4 model = glm::translate(this->model(), mPositions[i]);
    model = glm::rotate(model, glm::radians(20.f * i), glm::vec3(1.0f, 0.3f, 0.5f));
    models.push_back(model);
  }

  return models;
}

void Box::setupBuffers()
{
  glBindVertexArray(mMesh.vao());

  mModelsVBO = u_model::addModelsBuffer(models());

  glBindVertexArray(0);
}

void Box::render(const Shader& shader) const
{
  Lighting::u_material::bindTextures(shader, &mTextures[0], mTextures.size());

  mMesh.drawInstanced(mPositions.size());

  Lighting::u_material::unbindTextures(shader, &mTextures[0], mTextures.size());
}

void Box::free() const
{
  mMesh.free();

  Utils::Textures::deleteTextures(mTextures);

  glDeleteBuffers(1, &mModelsVBO);
}
