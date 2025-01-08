#include "grass.h"

#include "shaders/u-material.h"
#include "shaders/u-model.h"
#include "utils/utils.h"
#include <glm/gtc/matrix_transform.hpp>

Grass::Grass() :
  mPositions({
    { -1.5f, 0.0f, -0.48f },
    {  1.5f, 0.0f,  0.51f },
    {  0.0f, 0.0f,  0.70f },
    { -0.3f, 0.0f, -2.30f },
    {  0.5f, 0.0f, -0.60f },
}),
  mMesh(Shapes::QUAD),
  mTexture(Texture {
    .id = Utils::loadTexture2D("assets/grass.png", GL_CLAMP_TO_EDGE),
    .type = TEXTURE_TYPE_DIFFUSE,
    .key = "assets/grass.png",
  })
{
  setModel(glm::mat4(1.f));
  setupBuffers();
}

std::vector<glm::mat4> Grass::models() const
{
  std::vector<glm::mat4> models;
  for (unsigned int i = 0; i < mPositions.size(); i++) {
    models.push_back(glm::translate(Renderable::model(), mPositions[i]));
  }

  return models;
}

void Grass::setupBuffers() { u_model::setupInstancedModels(mMesh, models()); }

void Grass::render(const Shader& shader) const
{
  glDisable(GL_CULL_FACE);

  Lighting::u_material::bindTextures(shader, &mTexture);

  mMesh.drawInstanced(mPositions.size());

  Lighting::u_material::unbindTextures(shader, &mTexture);

  glEnable(GL_CULL_FACE);
}

void Grass::free() const
{
  mMesh.free();
  Utils::deleteTextures({ mTexture });
}
