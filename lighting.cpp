#include "lighting.h"
#include <iostream>

#define DIRECTIONAL_LIGHT
#define SPOT_LIGHT
#define POINT_LIGHT

Lighting::Lighting(Shader &shader) : mShader(shader)
{
  mShader.setFloat("u_material.shininess", 32.f);
  #ifdef DIRECTIONAL_LIGHT
  initDirLight();
  #else
  mShader.setBool("u_dir_light.light.off", true);
  #endif
  #ifdef SPOT_LIGHT
  initSpotLight();
  #else
  mShader.setBool("u_spot_light.light.off", true);
  #endif
  #ifdef POINT_LIGHT
  initPointLights();
  #else
  mShader.setBool("u_point_lights[0].light.off", true);
  mShader.setBool("u_point_lights[1].light.off", true);
  mShader.setBool("u_point_lights[2].light.off", true);
  mShader.setBool("u_point_lights[3].light.off", true);
  #endif
}

Lighting::~Lighting()
{
}

void Lighting::setViewPosition(glm::vec3 position)
{
  mShader.setVec3fv("u_view_pos", position);
}

void Lighting::initDirLight()
{
  const glm::vec3 AMBIENT(0.2f);
  const glm::vec3 DIFFUSE(0.5f);
  const glm::vec3 SPECULAR(1.0f);
  const glm::vec3 color(1.f);

  const glm::vec3 direction(-0.2f, -1.0f, -0.3f);
  const bool off = false;
  const glm::vec3 ambient = color * AMBIENT;
  const glm::vec3 diffuse = color * DIFFUSE;
  const glm::vec3 specular = color * SPECULAR;

  mShader.setVec3fv("u_dir_light.direction", direction);
  mShader.setBool("u_dir_light.off", off);
  mShader.setVec3fv("u_dir_light.light.ambient", ambient);
  mShader.setVec3fv("u_dir_light.light.diffuse", diffuse);
  mShader.setVec3fv("u_dir_light.light.specular", specular);
}

void Lighting::initPointLights()
{
  const glm::vec3 AMBIENT(0.2f);
  const glm::vec3 DIFFUSE(0.5f);
  const glm::vec3 SPECULAR(1.0f);

  const float aConstant = 1.f;
  const float aLinear = 0.09f;
  const float aQuadratic = 0.032f;

  for(unsigned int i = 0; i < NR_POINT_LIGHTS; i++)
  {
    std::stringstream key;
    key << "u_point_lights[" << i << "]";
    mShader.setVec3fv(key.str() + ".position", mLights.positions[i]);

    mShader.setFloat(key.str() + ".attenuation.contant", aConstant);
    mShader.setFloat(key.str() + ".attenuation.linear", aLinear);
    mShader.setFloat(key.str() + ".attenuation.quadratic", aQuadratic);

    mShader.setBool(key.str() + ".light.off", false);
    mShader.setVec3fv(key.str() + ".light.ambient", mLights.colors[i] * AMBIENT);
    mShader.setVec3fv(key.str() + ".light.diffuse", mLights.colors[i] * DIFFUSE);
    mShader.setVec3fv(key.str() + ".light.specular", mLights.colors[i] * SPECULAR);
  }
}

void Lighting::updatePointLight0Position()
{
  mShader.setVec3fv("u_point_lights[0].position", mLights.positions[0]);
}

void Lighting::initSpotLight()
{
  const glm::vec3 AMBIENT(0.2f);
  const glm::vec3 DIFFUSE(0.5f);
  const glm::vec3 SPECULAR(1.0f);
  const glm::vec3 color(1.f);

  const glm::vec3 direction = glm::vec3(0.0f);
  const float cutOff = glm::cos(glm::radians(12.5f));
  const float outerCutOff = glm::cos(glm::radians(17.5f));
  const glm::vec3 position = glm::vec3(0.f);

  const float aConstant = 1.f;
  const float aLinear = 0.09f;
  const float aQuadratic = 0.032f;
  const bool off = true;
  const glm::vec3 ambient = color * AMBIENT;
  const glm::vec3 diffuse = color * DIFFUSE;
  const glm::vec3 specular = color * SPECULAR;

  mShader.setVec3fv("u_spot_light.direction", direction);
  mShader.setFloat("u_spot_light.cutOff", cutOff);
  mShader.setFloat("u_spot_light.outerCutOff", outerCutOff);
  mShader.setVec3fv("u_spot_light.position", position);

  mShader.setFloat("u_spot_light.attenuation.contant", aConstant);
  mShader.setFloat("u_spot_light.attenuation.linear", aLinear);
  mShader.setFloat("u_spot_light.attenuation.quadratic", aQuadratic);

  mShader.setBool("u_spot_light.light.off", off);
  mShader.setVec3fv("u_spot_light.light.ambient", ambient);
  mShader.setVec3fv("u_spot_light.light.diffuse", diffuse);
  mShader.setVec3fv("u_spot_light.light.specular", specular);
}

void Lighting::updateSpotLight(glm::vec3 position, glm::vec3 direction, bool off)
{
  mShader.setVec3fv("u_spot_light.position", position);
  mShader.setVec3fv("u_spot_light.direction", direction);
  mShader.setBool("u_spot_light.light.off", off);
}
