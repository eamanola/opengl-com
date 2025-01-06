#include "grass.h"

#include "shaders/u-material.h"
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
}

void Grass::render(const Shader& shader) const
{
  glDisable(GL_CULL_FACE);

  Lighting::u_material::bindTextures(shader, &mTexture);

  for (unsigned int i = 0; i < mPositions.size(); i++) {
    glm::mat4 model = glm::translate(Renderable::model(), mPositions[i]);
    shader.setMat4fv("u_model", model);
    shader.setMat3fv("u_trans_inver_model", glm::mat3(glm::transpose(glm::inverse(model))));

    mMesh.draw();
  }

  Lighting::u_material::unbindTextures(shader, &mTexture);

  glEnable(GL_CULL_FACE);
}

void Grass::free() const
{
  mMesh.free();
  Utils::deleteTextures({ mTexture });
}
