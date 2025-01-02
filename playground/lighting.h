#ifndef LIGHTING_SHADER_H
#define LIGHTING_SHADER_H

#include <glm/glm.hpp>
#include <vector>
#include "../shader.h"
#include "../shaders/ub-lights-buffer.h"

class Lighting
{
public:
  Lighting(
    const std::vector<Shader>& shaders,
    unsigned int bindingId,
    unsigned int numDirLights,
    unsigned numPointLights,
    unsigned numSpotLights
  );
  ~Lighting() {}

  struct Lights {
    std::vector<glm::vec3> positions = {
      glm::vec3( 0.7f,  0.2f,  2.0f),
      glm::vec3( 2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f,  2.0f, -12.0f),
      glm::vec3( 0.0f,  0.0f, -2.0f)
    };
    std::vector<glm::vec4> colors = {
      glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
      glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
    };
  } mLights;

  // const unsigned int NR_POINT_LIGHTS = mLights.positions.size();

  void setup(Shader &shader);
  void setViewPosition(Shader &shader, const glm::vec3& position);
  void updatePointLight0Position();
  void updateSpotLight(const glm::vec3& position, const glm::vec3& direction, bool off);
  void free() { ubLightsBuffer.free(); }

private:
  std::vector<DirLight> getDirLights();
  std::vector<PointLight> getPointLights();
  std::vector<SpotLight> getSpotLights();
  UBLightsBuffer ubLightsBuffer;
};

#endif
