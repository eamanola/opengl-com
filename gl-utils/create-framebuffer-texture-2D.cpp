#include "gl-utils.h"

#include <cstring>
#include <iostream>

bool GLUtils::Framebuffer::addColorAttachment(
  unsigned int& outTextureId,
  GLenum attachement,
  const unsigned int samples,
  GLenum internal,
  const std::size_t width,
  const std::size_t height,
  GLenum format,
  GLenum type
)
{
  unsigned int textureId;
  glGenTextures(1, &textureId);
  if (samples < 2) {
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format, type, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, GL_TEXTURE_2D, textureId, 0);
  } else {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureId);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal, width, height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, attachement, GL_TEXTURE_2D_MULTISAMPLE, textureId, 0);
  }

  bool success = GLUtils::noErrors();
  outTextureId = success ? textureId : 0;
  if (!success) {
    glDeleteTextures(1, &textureId);
  }

  return success;
}

bool GLUtils::Framebuffer::addDepthStencilAttachment(
  unsigned int& outRBO,
  const unsigned int samples,
  const std::size_t width,
  const std::size_t height
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

bool GLUtils::Framebuffer::createFramebufferTexture2D(
  unsigned int& outFBO,
  unsigned int* outTextureIds,
  unsigned int texturesLen,
  unsigned int* outRBO,
  const unsigned int samples,
  GLenum internal,
  const std::size_t width,
  const std::size_t height,
  GLenum format,
  GLenum type
)
{
  unsigned int FBO = 0;
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  unsigned int textureIds[texturesLen];
  if (texturesLen > 0) {
    for (unsigned int i = 0; i < texturesLen; i++) {
      addColorAttachment(
        textureIds[i], GL_COLOR_ATTACHMENT0 + i, samples, internal, width, height, format, type
      );
    }
  } else {
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }

  unsigned int RBO = 0;
  if (outRBO != nullptr) {
    addDepthStencilAttachment(RBO, samples, width, height);
  }

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "frame buffer not complete\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (FBO != 0)
      glDeleteBuffers(1, &FBO);
    if (texturesLen > 0)
      glDeleteTextures(texturesLen, textureIds);
    if (RBO != 0)
      glDeleteRenderbuffers(1, &RBO);

    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  outFBO = FBO;
  if (texturesLen > 0) {
    memcpy(outTextureIds, textureIds, sizeof(textureIds));
  }
  if (outRBO != nullptr) {
    *outRBO = RBO;
  }

  return GLUtils::noErrors();
}
