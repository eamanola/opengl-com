#ifndef DISCO_LIGHTING_SETTINGS_H
#define DISCO_LIGHTING_SETTINGS_H

#include <vector>
#include <glm/glm.hpp>
#include "../shaders/lighted-shader.h"
#include "floor.h"
#include "../color.h"

class LightingSettings
{
public:
  LightingSettings();
  ~LightingSettings();

  struct SpotLights {
    std::vector<glm::vec3> positions = {
      glm::vec3( 2.5f, 1.5f, 0.f),
      glm::vec3(-2.5f, 1.5f, 0.f),
      // glm::vec3(  0.f, 1.5f, 2.5f)
    };
    std::vector<Color> colors = {
      Color(1.0f, 0.0f, 0.0f, 1.0f),
      Color(1.0f, 0.0f, 1.0f, 1.0f),
      // Color(1.0f, 1.0f, 1.0f, 1.0f)
    };
    std::vector<glm::vec3> directions = {
      glm::vec3(0.0f,  0.0f,  0.0f),
      glm::vec3(0.0f,  0.0f,  0.0f),
      // glm::vec3(0.0f, -1.5f, -2.5f)
    };
  } mSpotLights;

  const unsigned NR_DIR_LIGHTS = 1;
  const unsigned NR_POINT_LIGHTS = 25;
  const unsigned NR_SPOT_LIGHTS = 2;

  void setup(const LightedShader& shader);
  void update(float time);
  void initFloorLights(const LightedShader& shader, const Floor& floor);
  void updateFloorLights(const Shader& shader, const Floor& floor);

  void updateSpotLights(Shader &shader);

private:
  void initDirLight(const LightedShader& shader);
  void initSpotLights(const LightedShader& shader);
};

#endif
