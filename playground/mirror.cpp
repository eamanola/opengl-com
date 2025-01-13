#include "mirror.h"

#include "gl-utils/gl-utils.h"
#include "shaders/u-material.h"
#include "shaders/u-model.h"
#include "utils/utils.h"
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

#define SAMPLES 4

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
  mTexture(Texture { .type = TEXTURE_TYPE_DIFFUSE }),
  mWidth(vWidth),
  mHeight(vHeight)
{
  if (SAMPLES > 1) {
    // multisampled framebuffer
    GLUtils::Framebuffer::createFramebufferTexture2D(vWidth, vHeight, SAMPLES, mFBO, mTexI, &mRBO);

    // framebuffer to downscale/relove to, for drawing
    GLUtils::Framebuffer::createFramebufferTexture2D(
      vWidth, vHeight, 1, mFBOI, mTexture.id, nullptr
    );
  } else {
    GLUtils::Framebuffer::createFramebufferTexture2D(vWidth, vHeight, 1, mFBO, mTexture.id, &mRBO);
  }
}

void Mirror::screenshot(Scene& scene, const glm::vec3& positionOffset)
{
  const glm::mat4 m = model();
  const glm::vec3 position = this->position() + positionOffset;
  const glm::vec3 normal = glm::mat3(glm::transpose(glm::inverse(m))) * mNormal;
  Utils::screenshot(scene, mFBO, position, normal);

  // resolve framebuffer
  if (SAMPLES > 1) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBOI);
    glBlitFramebuffer(
      0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

void Mirror::screenshot(Scene& scene) { screenshot(scene, glm::vec3(0.f)); }

void Mirror::render(const Shader& shader) const
{
  u_model::setUModel(shader, model());

  Lighting::u_material::bindTexture(shader, &mTexture);
  mMesh.draw();
  Lighting::u_material::unbindTexture(shader, &mTexture);
}

void Mirror::free() const
{
  glDeleteFramebuffers(1, &mFBO);
  Utils::Textures::deleteTexture(mTexture);
  glDeleteRenderbuffers(1, &mRBO);

  if (SAMPLES > 1) {
    glDeleteFramebuffers(1, &mFBOI);
    glDeleteTextures(1, &mTexI);
  }
}
