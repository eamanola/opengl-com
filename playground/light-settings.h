#ifndef LIGHTING_SHADER_H
#define LIGHTING_SHADER_H

#include "shader.h"
#include "shaders/ub-lights.h"
#include <glm/glm.hpp>
#include <vector>

using Lighting::DirLight;
using Lighting::PointLight;
using Lighting::SpotLight;

class LightSettings
{
public:
  LightSettings(
    unsigned int bindingId,
    const std::vector<Shader>& shaders,
    unsigned int numDirLights,
    unsigned numPointLights,
    unsigned numSpotLights
  );
  ~LightSettings() { }

  struct {
    std::vector<glm::vec3> positions = {
      glm::vec3(0.7f, 0.2f, 2.0f),
      glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f, 2.0f, -12.0f),
      glm::vec3(0.0f, 1.0f, -2.0f),
    };
    std::vector<glm::vec4> colors = {
      glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
      glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
    };
  } mLights;

  // const unsigned int NR_POINT_LIGHTS = mLights.positions.size();

  void setup(const Shader& shader) { }
  void updatePointLight0Position();
  void updateSpotLight(const glm::vec3& position, const glm::vec3& direction, bool off);
  void free() { ub_lights.free(); }

private:
  std::vector<DirLight> getDirLights(unsigned int count);
  std::vector<PointLight> getPointLights(unsigned int count);
  std::vector<SpotLight> getSpotLights(unsigned int count);
  Lighting::ub_lights ub_lights;
};

#endif
