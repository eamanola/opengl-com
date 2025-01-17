#ifndef SHADOW_MAPS_H
#define SHADOW_MAPS_H

#include "scene.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <vector>

class ShadowMaps
{
private:
  /* data */
public:
  ShadowMaps(unsigned int numDirLights, unsigned int numPointLights, unsigned int numSpotLights);
  ~ShadowMaps();

  bool createBuffer(unsigned int& fbo, unsigned int& depthMap);
  bool createBuffer3D(unsigned int& fbo, unsigned int& depthMap);

  const glm::mat4 proj() const;
  const glm::mat4 view(const glm::vec3& lightPos, const glm::vec3& pointTo) const;
  const glm::mat4 light_space(const glm::vec3& lightPos, const glm::vec3& pointTo) const;

  const glm::mat4 proj_cube(const float& far) const;
  const std::vector<glm::mat4> view_cube(const glm::vec3& lightPos) const;
  const std::vector<glm::mat4> cube_space(const float& far, const glm::vec3& lightPos) const;

  bool updateDirMaps(
    const Scene& scene,
    const std::vector<glm::vec3>& lightPos,
    const std::vector<glm::vec3>& pointTo
  ) const;
  bool updateCubeMaps(const Scene& scene, const std::vector<glm::vec3>& lightPos, const float& far)
    const;

  const unsigned int WIDTH = 1024;
  const unsigned int HEIGHT = 1024;

  void free() const;

  void bindTextures(const Shader& shader, unsigned int first = 0) const;
  void unbindTextures(const Shader& shader, unsigned int first = 0) const;

  // unsigned int mFBO1;
  // unsigned int mDepthMap1;
  // unsigned int mFBO;
  // unsigned int mDepthMap;

private:
  std::vector<std::pair<unsigned int, unsigned int>> mDirLights;
  std::vector<std::pair<unsigned int, unsigned int>> mPointLights;
};

#endif
