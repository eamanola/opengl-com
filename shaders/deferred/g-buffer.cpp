#include "g-buffer.h"

#include "gl-utils/gl-utils.h"
#include "utils/utils.h"
#include <iostream>

GBuffer::GBuffer(const std::size_t width, const std::size_t height) :
  mTextures(7, Texture { .type = TEXTURE_TYPE_DIFFUSE })
{
  glGenFramebuffers(1, &mFbo);
  glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

  // can post process antialiase in
  const unsigned int SAMPLES = 1;

  // position vec3f
  GLUtils::Framebuffer::addColorAttachment(
    mTextures[0].id, GL_COLOR_ATTACHMENT0, SAMPLES, GL_RGB16F, width, height, GL_RGB, GL_FLOAT
  );

  // normal vec3f
  GLUtils::Framebuffer::addColorAttachment(
    mTextures[1].id, GL_COLOR_ATTACHMENT1, SAMPLES, GL_RGB16F, width, height, GL_RGB, GL_FLOAT
  );

  // color vec3
  GLUtils::Framebuffer::addColorAttachment(
    mTextures[2].id, GL_COLOR_ATTACHMENT2, SAMPLES, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE
  );

  // alpha float
  GLUtils::Framebuffer::addColorAttachment(
    mTextures[3].id, GL_COLOR_ATTACHMENT3, SAMPLES, GL_RED, width, height, GL_RED, GL_UNSIGNED_BYTE
  );

  // specular float
  GLUtils::Framebuffer::addColorAttachment(
    mTextures[4].id, GL_COLOR_ATTACHMENT4, SAMPLES, GL_RED, width, height, GL_RED, GL_UNSIGNED_BYTE
  );

  // emissive vec3
  GLUtils::Framebuffer::addColorAttachment(
    mTextures[5].id, GL_COLOR_ATTACHMENT5, SAMPLES, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE
  );

  // depth float
  GLUtils::Framebuffer::addColorAttachment(
    mTextures[6].id, GL_COLOR_ATTACHMENT6, SAMPLES, GL_RGB16F, width, height, GL_RGB, GL_FLOAT
  );

  GLUtils::Framebuffer::addDepthStencilAttachment(mRbo, SAMPLES, width, height);

  unsigned int attachments[7] = {
    GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6,
  };
  glDrawBuffers(7, attachments);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "g-buffer not complete\n";
    free();
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::free() const
{
  glDeleteFramebuffers(1, &mFbo);
  glDeleteBuffers(1, &mFbo);
  Utils::Textures::deleteTextures(mTextures);
  glDeleteRenderbuffers(1, &mRbo);
}
