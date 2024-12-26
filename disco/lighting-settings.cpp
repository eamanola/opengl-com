#include "lighting-settings.h"
#include <iostream>

#define DIRECTIONAL_LIGHT
#define SPOT_LIGHT
#define POINT_LIGHT

LightingSettings::LightingSettings()
{
}

LightingSettings::~LightingSettings()
{
}

void LightingSettings::setup(Shader &shader)
{
  shader.setFloat("u_material.shininess", 32.f);
  #ifdef DIRECTIONAL_LIGHT
  initDirLight(shader);
  #else
  shader.setBool("u_dir_lights[0].light.off", true);
  #endif
  #ifdef SPOT_LIGHT
  initSpotLight(shader);
  #else
  shader.setBool("u_spot_lights[0].light.off", true);
  #endif
  #ifdef POINT_LIGHT
  initPointLights(shader);
  #else
  shader.setBool("u_point_lights[0].light.off", true);
  shader.setBool("u_point_lights[1].light.off", true);
  shader.setBool("u_point_lights[2].light.off", true);
  shader.setBool("u_point_lights[3].light.off", true);
  #endif
}

void LightingSettings::setViewPosition(Shader &shader, const glm::vec3& position)
{
  shader.setVec3fv("u_view_pos", position);
}

void LightingSettings::initDirLight(Shader &shader)
{
  const glm::vec4 AMBIENT(0.2f, 0.2f, 0.2f, 1.f);
  const glm::vec4 DIFFUSE(0.5f, 0.5f, 0.5f, 1.f);
  const glm::vec4 SPECULAR(1.0f, 1.0f, 1.0f, 1.f);
  const glm::vec4 color(1.0f, 1.0f, 1.0f, 1.f);

  const glm::vec3 direction(0.f, -1.0f, 1.f);
  const bool off = false;
  const glm::vec4 ambient = color * AMBIENT;
  const glm::vec4 diffuse = color * DIFFUSE;
  const glm::vec4 specular = color * SPECULAR;

  shader.setVec3fv("u_dir_lights[0].direction", direction);
  shader.setBool("u_dir_lights[0].off", off);
  shader.setVec4fv("u_dir_lights[0].light.ambient", ambient);
  shader.setVec4fv("u_dir_lights[0].light.diffuse", diffuse);
  shader.setVec4fv("u_dir_lights[0].light.specular", specular);
}

void LightingSettings::initPointLights(Shader &shader)
{
  const glm::vec4 AMBIENT(0.2f, 0.2f, 0.2f, 0.f);
  const glm::vec4 DIFFUSE(0.5f, 0.5f, 0.5f, 0.f);
  const glm::vec4 SPECULAR(1.0f, 1.0f, 1.0f, 0.f);
  const glm::vec4 color(1.0f, 1.0f, 1.0f, 0.f);

  const float aConstant = 1.f;
  const float aLinear = 0.09f;
  const float aQuadratic = 0.032f;

  for(unsigned int i = 0; i < NR_POINT_LIGHTS; i++)
  {
    std::stringstream key;
    key << "u_point_lights[" << i << "]";
    shader.setVec3fv(key.str() + ".position", mLights.positions[i]);

    shader.setFloat(key.str() + ".attenuation.contant", aConstant);
    shader.setFloat(key.str() + ".attenuation.linear", aLinear);
    shader.setFloat(key.str() + ".attenuation.quadratic", aQuadratic);

    shader.setBool(key.str() + ".light.off", false);
    shader.setVec4fv(key.str() + ".light.ambient", mLights.colors[i] * AMBIENT);
    shader.setVec4fv(key.str() + ".light.diffuse", mLights.colors[i] * DIFFUSE);
    shader.setVec4fv(key.str() + ".light.specular", mLights.colors[i] * SPECULAR);
  }
}

void LightingSettings::initFloorLights(const Shader& shader, const Floor& floor)
{
  const glm::vec4 AMBIENT(0.2f, 0.2f, 0.2f, 0.f);
  const glm::vec4 DIFFUSE(0.5f, 0.5f, 0.5f, 0.f);
  const glm::vec4 SPECULAR(1.0f, 1.0f, 1.0f, 0.f);
  // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
  //  7, 1.0, 0.7, 1.8
  // 13, 1.0, 0.35, 0.44
  // 20, 1.0, 0.22, 0.20
  const float aConstant = 1.f;
  const float aLinear = 0.7f;
  const float aQuadratic = 1.8f;

  const std::vector<glm::vec3> positions = floor.positions();
  const std::vector<glm::vec4> colors = floor.colors();
  const glm::mat4 model = floor.model();

  for(unsigned int i = 0; i < positions.size(); i++)
  {
    std::stringstream key;
    key << "u_point_lights[" << i << "]";
    glm::vec3 position = glm::mat3(model) * positions[i];
    shader.setVec3fv(key.str() + ".position", position);

    shader.setFloat(key.str() + ".attenuation.contant", aConstant);
    shader.setFloat(key.str() + ".attenuation.linear", aLinear);
    shader.setFloat(key.str() + ".attenuation.quadratic", aQuadratic);

    shader.setBool(key.str() + ".light.off", false);
    shader.setVec4fv(key.str() + ".light.ambient", colors[i] * AMBIENT);
    shader.setVec4fv(key.str() + ".light.diffuse", colors[i] * DIFFUSE);
    shader.setVec4fv(key.str() + ".light.specular", colors[i] * SPECULAR);
  }
}

void LightingSettings::updateFloorLights(const Shader& shader, const Floor& floor)
{
  const glm::vec4 AMBIENT(0.2f, 0.2f, 0.2f, 0.f);
  const glm::vec4 DIFFUSE(0.5f, 0.5f, 0.5f, 0.f);
  const glm::vec4 SPECULAR(1.0f, 1.0f, 1.0f, 0.f);

  const std::vector<glm::vec4> colors = floor.colors();

  for(unsigned int i = 0; i < colors.size(); i++)
  {
    std::stringstream key;
    key << "u_point_lights[" << i << "]";
    shader.setVec4fv(key.str() + ".light.ambient", colors[i] * AMBIENT);
    shader.setVec4fv(key.str() + ".light.diffuse", colors[i] * DIFFUSE);
    shader.setVec4fv(key.str() + ".light.specular", colors[i] * SPECULAR);
  }
}

void LightingSettings::updatePointLight0Position(Shader &shader)
{
  shader.setVec3fv("u_point_lights[0].position", mLights.positions[0]);
}

void LightingSettings::initSpotLight(Shader &shader)
{
  const glm::vec4 AMBIENT(0.2f, 0.2f, 0.2f, 0.f);
  const glm::vec4 DIFFUSE(0.5f, 0.5f, 0.5f, 0.f);
  const glm::vec4 SPECULAR(1.0f, 1.0f, 1.0f, 0.f);
  const glm::vec4 color(1.0f, 1.0f, 1.0f, 0.f);

  const glm::vec3 direction = glm::vec3(0.0f);
  const float cutOff = glm::cos(glm::radians(12.5f));
  const float outerCutOff = glm::cos(glm::radians(17.5f));
  const glm::vec3 position = glm::vec3(0.f);

  const float aConstant = 1.f;
  const float aLinear = 0.09f;
  const float aQuadratic = 0.032f;
  const bool off = true;
  const glm::vec4 ambient = color * AMBIENT;
  const glm::vec4 diffuse = color * DIFFUSE;
  const glm::vec4 specular = color * SPECULAR;

  shader.setVec3fv("u_spot_lights[0].direction", direction);
  shader.setFloat("u_spot_lights[0].cutOff", cutOff);
  shader.setFloat("u_spot_lights[0].outerCutOff", outerCutOff);
  shader.setVec3fv("u_spot_lights[0].position", position);

  shader.setFloat("u_spot_lights[0].attenuation.contant", aConstant);
  shader.setFloat("u_spot_lights[0].attenuation.linear", aLinear);
  shader.setFloat("u_spot_lights[0].attenuation.quadratic", aQuadratic);

  shader.setBool("u_spot_lights[0].light.off", off);
  shader.setVec4fv("u_spot_lights[0].light.ambient", ambient);
  shader.setVec4fv("u_spot_lights[0].light.diffuse", diffuse);
  shader.setVec4fv("u_spot_lights[0].light.specular", specular);
}
