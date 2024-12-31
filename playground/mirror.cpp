#include "mirror.h"
#include <glad/gl.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "../shader-utils/shader-utils.h"

Mirror::Mirror(const float vWidth, const float vHeight)
:
mNormal(glm::normalize(glm::vec3(0.f, 0.f, 1.f))),
mMesh(
  std::vector<Vertex> ({
    // mirrored .texCoords.x
    { .position = { -2.5f, -2.5f * vHeight / vWidth, 0.f }, .normal = mNormal, .texCoords = { 1.f, 0.f } },
    { .position = {  2.5f, -2.5f * vHeight / vWidth, 0.f }, .normal = mNormal, .texCoords = { 0.f, 0.f } },
    { .position = {  2.5f,  2.5f * vHeight / vWidth, 0.f }, .normal = mNormal, .texCoords = { 0.f, 1.f } },
    { .position = { -2.5f,  2.5f * vHeight / vWidth, 0.f }, .normal = mNormal, .texCoords = { 1.f, 1.f } }
  }),
  std::vector<unsigned int> ({
    0, 1, 2,
    0, 2, 3
  })
),
mTexture(
  Texture {
    .type = TEXTURE_TYPE_DIFFUSE,
    .path = ""
  }
)
{
  ShaderUtils::createFramebufferTexture2D(vWidth, vHeight, mFBO, mTexture.id, mRBO);
}

Mirror::~Mirror()
{
}

void Mirror::screenshot(Scene &scene, const glm::vec3& positionOffset)
{
  const glm::mat4 m = model();
  const glm::vec3 position = glm::vec3(m[3]) + positionOffset;
  const glm::vec3 normal = glm::mat3(glm::transpose(glm::inverse(m))) * mNormal;
  ShaderUtils::screenshot(scene, mFBO, position, normal);
}

void Mirror::screenshot(Scene &scene)
{
  screenshot(scene, glm::vec3(0.f));
}

void Mirror::draw(const Shader& shader) const
{
  shader.setMat4fv("u_model", model());

  mMesh.draw(shader, &mTexture);
}

void Mirror::free() const
{
  glDeleteFramebuffers(1, &mFBO);
  glDeleteTextures(1, &mTexture.id);
  glDeleteRenderbuffers(1, &mRBO);
}
