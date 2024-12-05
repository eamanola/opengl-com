#ifndef LIGHTING_SHADER_H
#define LIGHTING_SHADER_H

#include "shader.h"
#include <glm/glm.hpp>

class LightingShader : public Shader
{
public:
  LightingShader(
    const char* vs = "./shaders/lighting.glvs",
    const char* fs = "./shaders/lighting.glfs"
  );
  ~LightingShader();

  struct Material
  {
    int texture_diffuse0;
    int texture_specular0;
    float shininess;
  };

  struct Attenuation {
    float constant;
    float linear;
    float quadratic;
  };

  struct Light {
    bool off = false;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
  };

  struct DirLight {
    glm::vec3 direction;

    Light light;
  };

  struct PointLight {
    glm::vec3 position;
    Attenuation attenuation;

    Light light;
  };

  struct SpotLight {
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

    glm::vec3 position;
    Attenuation attenuation;

    Light light;
  };

  void setViewPosition(glm::vec3 position);
  void setMaterial(Material material);
  void setDirLight(DirLight dirLight);
  void setPointLights(PointLight pointLights[], unsigned int count);
  void setPointLight0Position(glm::vec3 position);
  void setSpotLight(SpotLight spotLight);
  void updateSpotLight(glm::vec3 position, glm::vec3 direction, bool off);
  void setModel(glm::mat4 model);
  void setView(glm::mat4 view);
  void setProjection(glm::mat4 projection);

  const int NR_POINT_LIGHTS = 4;
};

#endif
