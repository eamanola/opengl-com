#include "grass.h"
#include "vertex.h"
#include "utils/utils.h"
#include "glm/gtc/matrix_transform.hpp"
Grass::Grass(/* args */)
:
mPositions({
  glm::vec3(-1.5f,  0.0f, -0.48f),
  glm::vec3( 1.5f,  0.0f,  0.51f),
  glm::vec3( 0.0f,  0.0f,  0.70f),
  glm::vec3(-0.3f,  0.0f, -2.30f),
  glm::vec3( 0.5f,  0.0f, -0.60f)
}),
mMesh(Shapes::QUAD),
mTexture(
  Texture {
    .id = Utils::loadTexture2D("assets/grass.png", GL_CLAMP_TO_EDGE),
    .type = TEXTURE_TYPE_DIFFUSE,
    .key = "assets/grass.png"
  }
)
{
  setModel(glm::mat4(1.f));
}

void Grass::draw(const Shader& shader) const
{
  glDisable(GL_CULL_FACE);
  for(unsigned int i = 0; i < mPositions.size(); i++)
  {
    glm::mat4 model = glm::translate(Drawable::model(), mPositions[i]);
    shader.setMat4fv("u_model", model);
    mMesh.draw(shader, &mTexture);
  }
  glEnable(GL_CULL_FACE);
}

void Grass::free() const
{
  mMesh.free();
  Utils::deleteTextures({ mTexture });
}
