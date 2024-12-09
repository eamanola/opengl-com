#ifndef LIGHTING_SHADER_H
#define LIGHTING_SHADER_H

#include <glm/glm.hpp>
#include "shader.h"

class Lighting
{
public:
  Lighting(Shader &shader);
  ~Lighting();

  struct Lights {
    glm::vec3 positions[4] = {
      glm::vec3( 0.7f,  0.2f,  2.0f),
      glm::vec3( 2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f,  2.0f, -12.0f),
      glm::vec3( 0.0f,  0.0f, -2.0f)
    };
    glm::vec3 colors[4] = {
      glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(1.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 1.0f)
    };
  };

  const unsigned int NR_POINT_LIGHTS = 4;
  Lights mLights;

  void setViewPosition(glm::vec3 position);
  void updatePointLight0Position();
  void updateSpotLight(glm::vec3 position, glm::vec3 direction, bool off);

private:
  void initDirLight();
  void initSpotLight();
  void initPointLights();
  Shader mShader;
};

#endif
