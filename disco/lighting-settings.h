#ifndef DISCO_LIGHTING_SETTINGS_H
#define DISCO_LIGHTING_SETTINGS_H

#include <glm/glm.hpp>
#include "../shader.h"
#include "floor.h"

class LightingSettings
{
public:
  LightingSettings();
  ~LightingSettings();

  struct Lights {
    glm::vec3 positions[4] = {
      glm::vec3( 0.7f,  0.2f,  2.0f),
      glm::vec3( 2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f,  2.0f, -12.0f),
      glm::vec3( 0.0f,  0.0f, 0.0f)
    };
    glm::vec4 colors[4] = {
      glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
      glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
      glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
      glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
    };
  };

  const unsigned int NR_POINT_LIGHTS = 4;
  Lights mLights;

  void setup(Shader &shader);
  void setViewPosition(Shader &shader, const glm::vec3& position);
  void updatePointLight0Position(Shader &shader);
  void initFloorLights(const Shader& shader, const Floor& floor);
  void updateFloorLights(const Shader& shader, const Floor& floor);
private:
  void initDirLight(Shader &shader);
  void initSpotLight(Shader &shader);
  void initPointLights(Shader &shader);
};

#endif
