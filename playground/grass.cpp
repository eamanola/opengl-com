#include "grass.h"
#include "../vertex.h"
#include "../shader-utils.h"
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
mMesh(
  std::vector<Vertex> ({
    { .position = { -0.5f, -0.5f, 0.f }, .normal = { 0.f, 0.f, 1.f }, .texCoords = { 0.f, 0.f } },
    { .position = {  0.5f, -0.5f, 0.f }, .normal = { 0.f, 0.f, 1.f }, .texCoords = { 1.f, 0.f } },
    { .position = {  0.5f,  0.5f, 0.f }, .normal = { 0.f, 0.f, 1.f }, .texCoords = { 1.f, 1.f } },
    { .position = { -0.5f,  0.5f, 0.f }, .normal = { 0.f, 0.f, 1.f }, .texCoords = { 0.f, 1.f } }
  }),
  std::vector<unsigned int> ({
    0, 1, 2,
    0, 2, 3
  })
)
{
  const char * path = "assets/grass.png";

  mTexture = Texture
  {
    .id = ShaderUtils::loadTexture(path, GL_CLAMP_TO_EDGE),
    .type = TEXTURE_TYPE_DIFFUSE,
    .path = path
  };

}

Grass::~Grass()
{
}

void Grass::draw(Shader& shader)
{
  glDisable(GL_CULL_FACE);
  for(unsigned int i = 0; i < mPositions.size(); i++)
  {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), mPositions[i]);
    shader.setMat4fv("u_model", model);
    mMesh.draw(shader, &mTexture);
  }
  glEnable(GL_CULL_FACE);
}

void Grass::free()
{
  mMesh.free();
  glDeleteTextures(1, &mTexture.id);
}
