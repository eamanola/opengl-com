#include "shadow-maps.h"

#include "gl-utils/gl-utils.h"
#include "shaders/locations.h"
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

ShadowMaps::ShadowMaps(
  unsigned int numDirLights, unsigned int numPointLights, unsigned int numSpotLights
)
{
  for (unsigned int i = 0; i < numDirLights; i++) {
    unsigned int fbo;
    unsigned int depthMap;
    if (createBuffer(fbo, depthMap)) {
      mDirLights.push_back({ fbo, depthMap });
    } else {
      std::cout << "failed to create buffers\n";
    }
  }

  for (unsigned int i = 0; i < numPointLights; i++) {
    unsigned int fbo;
    unsigned int depthMap;
    if (createBuffer3D(fbo, depthMap)) {
      mPointLights.push_back({ fbo, depthMap });
    } else {
      std::cout << "failed to create buffers\n";
    }
  }
}

ShadowMaps::~ShadowMaps() { }

bool ShadowMaps::createBuffer(unsigned int& fbo, unsigned int& depthMap)
{
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr
  );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  float borderColor[] = { 1.f, 1.f, 1.f, 1.f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindTexture(GL_TEXTURE_2D, 0);

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  bool success =
    glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && GLUtils::noErrors();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  if (!success) {
    glDeleteBuffers(1, &fbo);
    glDeleteTextures(1, &depthMap);
  }

  return success;
}

bool ShadowMaps::createBuffer3D(unsigned int& fbo, unsigned int& depthMap)
{
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);
  for (unsigned int i = 0; i < 6; i++) {
    glTexImage2D(
      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
      0,
      GL_DEPTH_COMPONENT,
      WIDTH,
      HEIGHT,
      0,
      GL_DEPTH_COMPONENT,
      GL_FLOAT,
      nullptr
    );
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
  // float borderColor[] = { 1.f, 1.f, 1.f, 1.f };
  // glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  bool success =
    glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && GLUtils::noErrors();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  if (!success) {
    glDeleteBuffers(1, &fbo);
    glDeleteTextures(1, &depthMap);
  }

  return success;
}

const glm::mat4 ShadowMaps::proj() const
{
  float near = 1.f;
  float far = 7.5f;

  return glm::ortho(-10.f, 10.f, -10.f, 10.f, near, far);
}

const glm::mat4 ShadowMaps::proj_cube(const float& far) const
{
  float near = 1.f;
  return glm::perspective(glm::radians(90.f), (float)WIDTH / (float)HEIGHT, near, far);
}

const glm::mat4 ShadowMaps::view(const glm::vec3& lightPos, const glm::vec3& pointTo) const
{
  return glm::lookAt(lightPos, pointTo, glm::vec3(0.f, 1.f, 0.f));
}

const std::vector<glm::mat4> ShadowMaps::view_cube(const glm::vec3& lightPos) const
{
  std::vector<glm::mat4> views;
  views.push_back(
    glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0))
  );
  views.push_back(
    glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0))
  );
  views.push_back(
    glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0))
  );
  views.push_back(
    glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0))
  );
  views.push_back(
    glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0))
  );
  views.push_back(
    glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0))
  );

  return views;
}

const glm::mat4 ShadowMaps::light_space(const glm::vec3& lightPos, const glm::vec3& pointTo) const
{
  return proj() * view(lightPos, pointTo);
}

const std::vector<glm::mat4>
ShadowMaps::cube_space(const float& far, const glm::vec3& lightPos) const
{
  const glm::mat4 projection = proj_cube(far);
  const std::vector<glm::mat4> views = view_cube(lightPos);

  std::vector<glm::mat4> proj_x_views;
  for (glm::mat4 v : views)
    proj_x_views.push_back(projection * v);

  return proj_x_views;
}

bool ShadowMaps::updateDirMaps(
  const Scene& scene, const std::vector<glm::vec3>& lightPos, const std::vector<glm::vec3>& pointTo
) const
{
  glViewport(0, 0, WIDTH, HEIGHT);
  for (unsigned int i = 0; i < mDirLights.size(); i++) {
    glBindFramebuffer(GL_FRAMEBUFFER, mDirLights[i].first);
    glClear(GL_DEPTH_BUFFER_BIT);

    // glm::vec3 lightPos = glm::vec3(-2.f, 4.f, -1.f);
    // glm::vec3 pointTo = glm::vec3(0.f);
    // glm::vec3 direction = glm::normalize(pointTo - lightPos)

    scene.renderShadowMap(proj(), view(lightPos[i], pointTo[i]));
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, 800, 600);
  return GLUtils::noErrors();
}

bool ShadowMaps::updateCubeMaps(
  const Scene& scene, const std::vector<glm::vec3>& lightPos, const float& far
) const
{
  glViewport(0, 0, WIDTH, HEIGHT);
  for (unsigned int i = 0; i < mPointLights.size(); i++) {
    glBindFramebuffer(GL_FRAMEBUFFER, mPointLights[i].first);
    glClear(GL_DEPTH_BUFFER_BIT);

    scene.renderCubeMap(cube_space(far, lightPos[i]), lightPos[i], far);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, 800, 600);
  return GLUtils::noErrors();
}
void ShadowMaps::free() const
{
  std::vector<unsigned int> frameBuffers;
  std::vector<unsigned int> textures;

  for (std::pair pair : mDirLights) {
    frameBuffers.push_back(pair.first);
    textures.push_back(pair.second);
  }

  for (std::pair pair : mPointLights) {
    frameBuffers.push_back(pair.first);
    textures.push_back(pair.second);
  }

  glDeleteFramebuffers(frameBuffers.size(), &frameBuffers[0]);
  glDeleteTextures(textures.size(), &textures[0]);
}

void ShadowMaps::bindTextures() const
{
  unsigned int start = LOCATIONS::TEXTURES::SHADOWMAPS0;

  for (unsigned int i = 0; i < mDirLights.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i + start);
    glBindTexture(GL_TEXTURE_2D, mDirLights[i].second);
  }
  start += mDirLights.size();

  for (unsigned int i = 0; i < mPointLights.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i + start);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mPointLights[i].second);
  }
  // start += mPointLights.size();

  glActiveTexture(GL_TEXTURE0);
}

void ShadowMaps::unbindTextures() const
{
  unsigned int start = LOCATIONS::TEXTURES::SHADOWMAPS0;

  for (unsigned int i = 0; i < mDirLights.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i + start);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  start += mDirLights.size();

  for (unsigned int i = 0; i < mPointLights.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i + start);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  }
  // start += mPointLights.size();

  glActiveTexture(GL_TEXTURE0);
}
