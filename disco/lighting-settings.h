#ifndef DISCO_LIGHTING_SETTINGS_H
#define DISCO_LIGHTING_SETTINGS_H

#include <vector>
#include <glm/glm.hpp>
#include "../shader.h"
#include "floor.h"
#include "../color.h"

#define NR_LIGHTS 4
#define NR_SPOTLIGHTS 2
class LightingSettings
{
public:
  LightingSettings();
  ~LightingSettings();

  struct Lights {
    std::vector<glm::vec3> positions = {
      glm::vec3( 0.7f,  0.2f,   2.0f),
      glm::vec3( 2.3f, -3.3f,  -4.0f),
      glm::vec3(-4.0f,  2.0f, -12.0f),
      glm::vec3( 0.0f,  0.0f,   0.0f)
    };
    std::vector<Color> colors = {
      Color(1.0f, 1.0f, 1.0f, 1.0f),
      Color(1.0f, 0.0f, 0.0f, 1.0f),
      Color(0.0f, 1.0f, 0.0f, 1.0f),
      Color(0.0f, 0.0f, 1.0f, 1.0f)
    };
  };

  struct SpotLights {
    std::vector<glm::vec3> positions = {
      glm::vec3( 2.5f, 1.5f, 0.f),
      glm::vec3(-2.5f, 1.5f, 0.f)
    };
    std::vector<Color> colors = {
      Color(1.0f, 0.0f, 0.0f, 1.0f),
      Color(1.0f, 0.0f, 1.0f, 1.0f)
    };
    std::vector<glm::vec3> directions = {
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f)
    };
  };

  Lights mLights;
  SpotLights mSpotLights;
  const unsigned int NR_POINT_LIGHTS = NR_LIGHTS;
  const unsigned int NR_SPOT_LIGHTS = NR_SPOTLIGHTS;

  void setup(Shader &shader);
  void setViewPosition(Shader &shader, const glm::vec3& position);
  void updatePointLight0Position(Shader &shader);
  void initFloorLights(const Shader& shader, const Floor& floor);
  void updateFloorLights(const Shader& shader, const Floor& floor);

  void toggelPointLights1to4(const Shader& shader, bool off);
  void updateSpotLights(Shader &shader, bool off = false);

private:
  void initDirLight(Shader &shader);
  void initSpotLights(Shader &shader);
  void initPointLights(Shader &shader);
};

#endif
