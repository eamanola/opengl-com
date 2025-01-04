#include "gl-utils.h"
#include <iostream>

bool GLUtils::createFramebufferTexture2D(
  const float width, const float height,
  unsigned int &outFBO, unsigned int &outTextureId, unsigned int &outRBO
)
{
  glGenFramebuffers(1, &outFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, outFBO);

  glGenTextures(1, &outTextureId);
  glBindTexture(GL_TEXTURE_2D, outTextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outTextureId, 0);

  glGenRenderbuffers(1, &outRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, outRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, outRBO);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "frame buffer not complete\n";
    glDeleteBuffers(1, &outFBO);
    glDeleteTextures(1, &outTextureId);
    glDeleteRenderbuffers(1, &outRBO);
    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return true;
}
