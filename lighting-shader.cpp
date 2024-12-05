#include "lighting-shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

LightingShader::LightingShader(const char* vs, const char* fs) : Shader(vs, fs)
{
  use();
  setFloat("material.shininess", 32.f);
  setBool("dirLight.light.off", true);
  setBool("pointLights[0].light.off", true);
  setBool("pointLights[1].light.off", true);
  setBool("pointLights[2].light.off", true);
  setBool("pointLights[3].light.off", true);
  setBool("spotLight.light.off", true);
  use(false);
}

LightingShader::~LightingShader()
{
}

void LightingShader::setViewPosition(glm::vec3 position)
{
  setVec3("viewPos", (float *)glm::value_ptr(position));
}

void LightingShader::setMaterial(Material material)
{

}

void LightingShader::setDirLight(DirLight dirLight)
{
  setVec3("dirLight.direction", (float*)glm::value_ptr(dirLight.direction));
  setBool("dirLight.off", dirLight.light.off);
  setVec3("dirLight.light.ambient", (float *)glm::value_ptr(dirLight.light.ambient));
  setVec3("dirLight.light.diffuse", (float *)glm::value_ptr(dirLight.light.diffuse));
  setVec3("dirLight.light.specular", (float *)glm::value_ptr(dirLight.light.specular));
}

void LightingShader::setPointLights(PointLight pointLights[], unsigned int count)
{
  unsigned int max = NR_POINT_LIGHTS;
  if (count < max) max = count;
  else if (count > max) std::cout << "Using only " << NR_POINT_LIGHTS << " lights";
  for(unsigned int i = 0; i < max; i++)
  {
    PointLight pl = pointLights[i];
    std::stringstream key;
    key << "pointLights[" << i << "]";
    setVec3(key.str() + ".position", (float *)glm::value_ptr(pl.position));

    setFloat(key.str() + ".attenuation.contant", pl.attenuation.constant);
    setFloat(key.str() + ".attenuation.linear", pl.attenuation.linear);
    setFloat(key.str() + ".attenuation.quadratic", pl.attenuation.quadratic);

    setBool(key.str() + ".light.off", pl.light.off);
    setVec3(key.str() + ".light.ambient", (float *)glm::value_ptr(pl.light.ambient));
    setVec3(key.str() + ".light.diffuse", (float *)glm::value_ptr(pl.light.diffuse));
    setVec3(key.str() + ".light.specular", (float *)glm::value_ptr(pl.light.specular));
  }
}

void LightingShader::setPointLight0Position(glm::vec3 position)
{
  setVec3("pointLights[0].position", (float *)glm::value_ptr(position));
}

void LightingShader::setSpotLight(SpotLight spotLight)
{
  setVec3("spotLight.direction", (float*)glm::value_ptr(spotLight.direction));
  setFloat("spotLight.cutOff", spotLight.cutOff);
  setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
  setVec3("spotLight.position", (float *)glm::value_ptr(spotLight.position));

  setFloat("spotLight.attenuation.contant", spotLight.attenuation.constant);
  setFloat("spotLight.attenuation.linear", spotLight.attenuation.linear);
  setFloat("spotLight.attenuation.quadratic", spotLight.attenuation.quadratic);

  setBool("spotLight.light.off", spotLight.light.off);
  setVec3("spotLight.light.ambient", (float *)glm::value_ptr(spotLight.light.ambient));
  setVec3("spotLight.light.diffuse", (float *)glm::value_ptr(spotLight.light.diffuse));
  setVec3("spotLight.light.specular", (float *)glm::value_ptr(spotLight.light.specular));
}

void LightingShader::updateSpotLight(glm::vec3 position, glm::vec3 direction, bool off)
{
  setVec3("spotLight.position", (float *)glm::value_ptr(position));
  setVec3("spotLight.direction", (float*)glm::value_ptr(direction));
  setBool("spotLight.light.off", off);
}

void LightingShader::setModel(glm::mat4 model)
{
  setMat4fv("model", glm::value_ptr(model));
}

void LightingShader::setView(glm::mat4 view)
{
  setMat4fv("view", glm::value_ptr(view));
}

void LightingShader::setProjection(glm::mat4 projection)
{
  setMat4fv("projection", glm::value_ptr(projection));
}
