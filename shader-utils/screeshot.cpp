#include "shader-utils.h"
#include <iostream>

bool ShaderUtils::createFramebufferTexture2D(
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

void ShaderUtils::screenshot(
  Scene &scene, const unsigned& FBO, const glm::vec3& position, const glm::vec3& normal
)
{
  Camera& camera = scene.camera();
  const glm::vec3 cameraPos = camera.position();
  const glm::vec3 cameraDir = camera.front();

  const glm::vec3 incident = glm::normalize(position - cameraPos);
  const glm::vec3 reflection = glm::reflect(incident, normal);

  camera.setPosition(position);
  camera.setDirection(reflection);

  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  scene.render();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  camera.setPosition(cameraPos);
  camera.setDirection(cameraDir);
}
