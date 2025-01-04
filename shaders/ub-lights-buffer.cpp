#include "ub-lights-buffer.h"
#include <glm/gtc/type_ptr.hpp>

UBLightsBuffer::UBLightsBuffer(
  const unsigned int bindingId,
  const std::vector<Shader>& shaders,
  const unsigned int numDirLights,
  const unsigned int numPointLights,
  const unsigned int numSpotLights
)
:
UniformBlockBuffer(
  bindingId,
  shaders,
  "ub_lights",
  getUniformNames(numDirLights, numPointLights, numSpotLights)
)
{
}

bool UBLightsBuffer::set(const UBLights& lights) const
{
  char padded[mBlockDataSize] = {};

  copyDirLights(lights, padded);
  copyPointLights(lights, padded);
  copySpotLights(lights, padded);

  return UniformBlockBuffer::overwrite(sizeof(padded), padded);
}

bool UBLightsBuffer::setVec3(const char* uniformName, const glm::vec3& value) const
{
  return update(uniformName, sizeof(glm::vec3), glm::value_ptr(value));
}

bool UBLightsBuffer::setBool(const char* uniformName, const bool& value) const
{
  return update(uniformName, sizeof(bool), &value);
}

bool UBLightsBuffer::setColor(const char* uniformName, const Color& value) const
{
  return update(uniformName, sizeof(Color), glm::value_ptr(value));
}

std::vector<std::string> UBLightsBuffer::getUniformNames(
  unsigned int numDirLights, unsigned int numPointLights, unsigned int numSpotLights
) const
{
  std::vector<std::string> uniformNames;
  getDirLightUniformNames(numDirLights, uniformNames);
  getPointLightUniformNames(numPointLights, uniformNames);
  getSpotLightUniformNames(numSpotLights, uniformNames);

  return uniformNames;
}

void UBLightsBuffer::getLightUniformNames(
  const std::string& prefix, std::vector<std::string>& uniformNames
) const
{
  uniformNames.reserve(uniformNames.size() + 4);
  uniformNames.push_back(prefix + ".color.ambient");
  uniformNames.push_back(prefix + ".color.diffuse");
  uniformNames.push_back(prefix + ".color.specular");
  uniformNames.push_back(prefix + ".off");
}

void UBLightsBuffer::getAttenuationUniformNames(
  const std::string& prefix, std::vector<std::string>& uniformNames
) const
{
  uniformNames.reserve(uniformNames.size() + 3);
  uniformNames.push_back(prefix + ".constant");
  uniformNames.push_back(prefix + ".linear");
  uniformNames.push_back(prefix + ".quadratic");
}

void UBLightsBuffer::getDirLightUniformNames(
  const unsigned int numDirLights, std::vector<std::string>& uniformNames
) const
{
  uniformNames.reserve(uniformNames.size() + numDirLights * 1);
  for(unsigned int i = 0; i < numDirLights; i++)
  {
    std::string prefix = "u_dir_lights[" + std::to_string(i) + "]";

    uniformNames.push_back(prefix + ".direction");

    getLightUniformNames(prefix + ".light", uniformNames);
  }
}

void UBLightsBuffer::getPointLightUniformNames(
  const unsigned int numPointLights, std::vector<std::string>& uniformNames
) const
{
  uniformNames.reserve(uniformNames.size() + numPointLights * 1);
  for(unsigned int i = 0; i < numPointLights; i ++)
  {
    std::string prefix = "u_point_lights[" + std::to_string(i) + "]";

    uniformNames.push_back(prefix + ".position");

    getAttenuationUniformNames(prefix + ".attenuation", uniformNames);
    getLightUniformNames(prefix + ".light", uniformNames);
  }
}

void UBLightsBuffer::getSpotLightUniformNames(
  const unsigned int numSpotLights, std::vector<std::string>& uniformNames
) const
{
  uniformNames.reserve(uniformNames.size() + numSpotLights * 4);
  for(unsigned int i = 0; i < numSpotLights; i ++)
  {
    std::string prefix = "u_spot_lights[" + std::to_string(i) + "]";

    uniformNames.push_back(prefix + ".direction");
    uniformNames.push_back(prefix + ".cutOff");
    uniformNames.push_back(prefix + ".outerCutOff");
    uniformNames.push_back(prefix + ".position");

    getAttenuationUniformNames(prefix + ".attenuation", uniformNames);
    getLightUniformNames(prefix + ".light", uniformNames);
  }
}

void UBLightsBuffer::copyDirLights(const UBLights& lights, char* padded) const
{
  for(unsigned int i = 0; i < lights.dirLights.size(); i++)
  {
    copyDirLight(i, lights.dirLights[i], padded);
  }
}

void UBLightsBuffer::copyPointLights(const UBLights& lights, char* padded) const
{
  for(unsigned int i = 0; i < lights.pointLights.size(); i++)
  {
    copyPointLight(i, lights.pointLights[i], padded);
  }
}

void UBLightsBuffer::copySpotLights(const UBLights& lights, char* padded) const
{
  for(unsigned int i = 0; i < lights.spotLights.size(); i++)
  {
    copySpotLight(i, lights.spotLights[i], padded);
  }
}

void UBLightsBuffer::copyLight(const std::string& prefix, Light light, char* padded) const
{
  memcpy(&padded[mOffsets.at(prefix + ".color.ambient")], glm::value_ptr(light.color.ambient), sizeof(Color));
  memcpy(&padded[mOffsets.at(prefix + ".color.diffuse")], glm::value_ptr(light.color.diffuse), sizeof(Color));
  memcpy(&padded[mOffsets.at(prefix + ".color.specular")], glm::value_ptr(light.color.specular), sizeof(Color));
  memcpy(&padded[mOffsets.at(prefix + ".off")], &light.off, sizeof(bool));
}

void UBLightsBuffer::copyAttenuation(const std::string& prefix, Attenuation attenuation, char* padded) const
{
  memcpy(&padded[mOffsets.at(prefix + ".constant")], &attenuation.constant, sizeof(float));
  memcpy(&padded[mOffsets.at(prefix + ".linear")], &attenuation.linear, sizeof(float));
  memcpy(&padded[mOffsets.at(prefix + ".quadratic")], &attenuation.quadratic, sizeof(float));
}

void UBLightsBuffer::copyDirLight(const unsigned int index, const DirLight& dirLight, char* padded) const
{
  const std::string prefix = "u_dir_lights[" + std::to_string(index) + "]";
  copyLight(prefix + ".light", dirLight.light, padded);
}

void UBLightsBuffer::copyPointLight(const unsigned int index, const PointLight& pointLight, char* padded) const
{
  const std::string prefix = "u_point_lights[" + std::to_string(index) + "]";

  memcpy(&padded[mOffsets.at(prefix + ".position")], glm::value_ptr(pointLight.position), sizeof(glm::vec3));
  copyAttenuation(prefix + ".attenuation", pointLight.attenuation, padded);
  copyLight(prefix + ".light", pointLight.light, padded);
}

void UBLightsBuffer::copySpotLight(const unsigned int index, const SpotLight& spotLight, char* padded) const
{
  const std::string prefix = "u_spot_lights[" + std::to_string(index) + "]";

  memcpy(&padded[mOffsets.at(prefix + ".direction")], glm::value_ptr(spotLight.direction), sizeof(glm::vec3));
  memcpy(&padded[mOffsets.at(prefix + ".cutOff")], &spotLight.cutOff, sizeof(float));
  memcpy(&padded[mOffsets.at(prefix + ".outerCutOff")], &spotLight.outerCutOff, sizeof(float));
  memcpy(&padded[mOffsets.at(prefix + ".position")], glm::value_ptr(spotLight.position), sizeof(glm::vec3));

  copyAttenuation(prefix + ".attenuation", spotLight.attenuation, padded);
  copyLight(prefix + ".light", spotLight.light, padded);
}