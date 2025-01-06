#include "mirror.h"

#include "gl-utils/gl-utils.h"
#include "shaders/u-material.h"
#include "utils/utils.h"
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

Mirror::Mirror(const float vWidth, const float vHeight) :
  mNormal(glm::normalize(glm::vec3(0.f, 0.f, 1.f))),
  mMesh(
    // clang-format off
    std::vector<Vertex> ({
      // mirrored .texCoords.x
      { .position = { -2.5f, -2.5f * vHeight / vWidth, 0.f }, .normal = mNormal, .texCoords = { 1.f, 0.f } },
      { .position = {  2.5f, -2.5f * vHeight / vWidth, 0.f }, .normal = mNormal, .texCoords = { 0.f, 0.f } },
      { .position = {  2.5f,  2.5f * vHeight / vWidth, 0.f }, .normal = mNormal, .texCoords = { 0.f, 1.f } },
      { .position = { -2.5f,  2.5f * vHeight / vWidth, 0.f }, .normal = mNormal, .texCoords = { 1.f, 1.f } },
    }),
    std::vector<unsigned int> ({
      0, 1, 2,
      0, 2, 3
    })
    // clang-format on
  ),
  mTexture(Texture { .type = TEXTURE_TYPE_DIFFUSE, .key = "" })
{
  GLUtils::createFramebufferTexture2D(vWidth, vHeight, mFBO, mTexture.id, mRBO);
}

void Mirror::screenshot(Scene& scene, const glm::vec3& positionOffset)
{
  const glm::mat4 m = model();
  const glm::vec3 position = glm::vec3(m[3]) + positionOffset;
  const glm::vec3 normal = glm::mat3(glm::transpose(glm::inverse(m))) * mNormal;
  Utils::screenshot(scene, mFBO, position, normal);
}

void Mirror::screenshot(Scene& scene) { screenshot(scene, glm::vec3(0.f)); }

void Mirror::render(const Shader& shader) const
{
  shader.setMat4fv("u_model", model());
  shader.setMat3fv("u_trans_inver_model", glm::mat3(glm::transpose(glm::inverse(model()))));

  UMaterial::setTextures(shader, &mTexture);
  mMesh.draw();
  UMaterial::clearTextures(shader, &mTexture);
}

void Mirror::free() const
{
  glDeleteFramebuffers(1, &mFBO);
  Utils::deleteTextures({ mTexture });
  glDeleteRenderbuffers(1, &mRBO);
}
