#include "gl-utils.h"
#include <iostream>

bool GLUtils::Framebuffer::createFramebufferTexture2D(
  const float width,
  const float height,
  const unsigned int samples,
  unsigned int& outFBO,
  unsigned int& outTextureId,
  unsigned int* outRBO
)
{
  unsigned int FBO = 0;
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  unsigned int textureId = 0;
  addTexture(width, height, samples, textureId);

  unsigned int RBO = 0;
  if (outRBO != nullptr) {
    addRenderBuffer(width, height, samples, RBO);
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "frame buffer not complete\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (FBO != 0)
      glDeleteBuffers(1, &FBO);
    if (textureId != 0)
      glDeleteTextures(1, &textureId);
    if (RBO != 0)
      glDeleteRenderbuffers(1, &RBO);
    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  outFBO = FBO;
  outTextureId = textureId;
  if (outRBO != nullptr) {
    *outRBO = RBO;
  }

  return GLUtils::noErrors();
}

bool GLUtils::Framebuffer::addTexture(
  const float width, const float height, const unsigned int samples, unsigned int& outTextureId
)
{
  unsigned int textureId;
  glGenTextures(1, &textureId);
  if (samples < 2) {
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
  } else {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureId, 0
    );
  }

  bool success = GLUtils::noErrors();
  outTextureId = success ? textureId : 0;
  if (!success) {
    glDeleteTextures(1, &textureId);
  }

  return success;
}

bool GLUtils::Framebuffer::addRenderBuffer(
  const float width, const float height, const unsigned int samples, unsigned int& outRBO
)
{
  unsigned int RBO;
  glGenRenderbuffers(1, &RBO);
  glBindRenderbuffer(GL_RENDERBUFFER, RBO);
  if (samples < 2) {
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  } else {
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
  }
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

  bool success = GLUtils::noErrors();
  outRBO = success ? RBO : 0;
  if (!success) {
    glDeleteRenderbuffers(1, &RBO);
  }

  return success;
}
