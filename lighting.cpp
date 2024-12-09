#include "lighting.h"
#include <iostream>

#define DIRECTIONAL_LIGHT
#define SPOT_LIGHT
#define POINT_LIGHT

Lighting::Lighting(Shader &shader) : mShader(shader)
{
  mShader.setFloat("material.shininess", 32.f);
  #ifdef DIRECTIONAL_LIGHT
  initDirLight();
  #else
  mShader.setBool("dirLight.light.off", true);
  #endif
  #ifdef SPOT_LIGHT
  initSpotLight();
  #else
  mShader.setBool("spotLight.light.off", true);
  #endif
  #ifdef POINT_LIGHT
  initPointLights();
  #else
  mShader.setBool("pointLights[0].light.off", true);
  mShader.setBool("pointLights[1].light.off", true);
  mShader.setBool("pointLights[2].light.off", true);
  mShader.setBool("pointLights[3].light.off", true);
  #endif
}

Lighting::~Lighting()
{
}

void Lighting::setViewPosition(glm::vec3 position)
{
  mShader.setVec3fv("viewPos", position);
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

  mShader.setVec3fv("dirLight.direction", direction);
  mShader.setBool("dirLight.off", off);
  mShader.setVec3fv("dirLight.light.ambient", ambient);
  mShader.setVec3fv("dirLight.light.diffuse", diffuse);
  mShader.setVec3fv("dirLight.light.specular", specular);
}

void Lighting::initPointLights()
{

  // #ifdef POINT_LIGHT
  // for(unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
  // {
  //   pointLights[i] = {
  //     .position = lightPositions[i],
  //     .attenuation = { .constant = 1.f, .linear = 0.09f, .quadratic = 0.032f },
  //     .light = {
  //       .ambient = lightColors[i] * LIGHT_AMBIENT,
  //       .diffuse = lightColors[i] * LIGHT_DIFFUSE,
  //       .specular = lightColors[i] * LIGHT_SPECULAR,
  //     }
  //   };
  // }

  // program->setPointLights(pointLights, NUM_POINT_LIGHTS);
  // #endif

  const glm::vec3 AMBIENT(0.2f);
  const glm::vec3 DIFFUSE(0.5f);
  const glm::vec3 SPECULAR(1.0f);

  const float aConstant = 1.f;
  const float aLinear = 0.09f;
  const float aQuadratic = 0.032f;

  for(unsigned int i = 0; i < NR_POINT_LIGHTS; i++)
  {
    std::stringstream key;
    key << "pointLights[" << i << "]";
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
  mShader.setVec3fv("pointLights[0].position", mLights.positions[0]);
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

  mShader.setVec3fv("spotLight.direction", direction);
  mShader.setFloat("spotLight.cutOff", cutOff);
  mShader.setFloat("spotLight.outerCutOff", outerCutOff);
  mShader.setVec3fv("spotLight.position", position);

  mShader.setFloat("spotLight.attenuation.contant", aConstant);
  mShader.setFloat("spotLight.attenuation.linear", aLinear);
  mShader.setFloat("spotLight.attenuation.quadratic", aQuadratic);

  mShader.setBool("spotLight.light.off", off);
  mShader.setVec3fv("spotLight.light.ambient", ambient);
  mShader.setVec3fv("spotLight.light.diffuse", diffuse);
  mShader.setVec3fv("spotLight.light.specular", specular);
}

void Lighting::updateSpotLight(glm::vec3 position, glm::vec3 direction, bool off)
{
  mShader.setVec3fv("spotLight.position", position);
  mShader.setVec3fv("spotLight.direction", direction);
  mShader.setBool("spotLight.light.off", off);
}
