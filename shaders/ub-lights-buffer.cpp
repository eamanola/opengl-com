#include "ub-lights-buffer.h"
#include <glm/gtc/type_ptr.hpp>

UBLightsBuffer::UBLightsBuffer(
  const std::vector<Shader>& shaders,
  const unsigned int bindingId,
  const unsigned int numDirLights,
  const unsigned int numPointLights,
  const unsigned int numSpotLights
)
{
  const char* blockName = "ub_lights";

  std::vector<std::string> uniformNames;
  getDirLightUniformNames(numDirLights, uniformNames);
  getPointLightUniformNames(numPointLights, uniformNames);
  getSpotLightUniformNames(numSpotLights, uniformNames);

  if(uniformNames.size() == 0)
  {
    std::cout << "no uniforms to bind";
    return;
  }

  if(shaders.size() == 0)
  {
    std::cout << "no shaders to bind";
  }

  const unsigned int shaderId = shaders[0].id();

  std::vector<unsigned int> uniformBlockIndices;
  uniformBlockIndices.reserve(shaders.size());
  for(Shader shader: shaders)
  {
    unsigned int blockIndex;
    if(!UniformBlock::getBlockIndex(shader.id(), blockName, blockIndex))
    {
      std::cout << "Failed to find block\n";
      return;
    }
    uniformBlockIndices.push_back(blockIndex);
  }

  for(unsigned int i = 0; i < shaders.size(); i++)
  {
    if(!UniformBlock::bindBlock(bindingId, shaders[i].id(), uniformBlockIndices[i]))
    {
      std::cout << "Failed to bind shaders \n";
      free();
      return;
    }
  }

  if(!UniformBlock::getBlockDataSize(shaderId, uniformBlockIndices[0], mBlockDataSize))
  {
    std::cout << "Failed to get block data size\n";
    return;
  }

  if(!UniformBlockBuffer::createBuffer(mBlockDataSize, mBufferId))
  {
    std::cout << "Failed to create buffer\n";
    free();
    return;
  }

  if(!UniformBlockBuffer::bindBuffer(bindingId, mBufferId))
  {
    std::cout << "Failed to bind buffer\n";
    free();
    return;
  }

  int offsets[uniformNames.size()];
  if(!UniformBlock::getUniformOffsets(shaderId, uniformNames, offsets))
  {
    std::cout << "Failed to get offsets, error:\n";
    free();
    return;
  }

  mOffsets.reserve(uniformNames.size());
  for(unsigned int i = 0; i < uniformNames.size(); i++)
  {
    mOffsets[uniformNames[i]] = offsets[i];
  }
}

bool UBLightsBuffer::set(const UBLights& lights)
{
  char padded[mBlockDataSize] = {};

  copyDirLights(lights, padded);
  copyPointLights(lights, padded);
  copySpotLights(lights, padded);

  return UniformBlockBuffer::overwrite(mBufferId, sizeof(padded), padded);
}

bool UBLightsBuffer::setVec3(const char* key, const glm::vec3& value)
{
  return UniformBlockBuffer::set(mBufferId, mOffsets, key, sizeof(glm::vec3), glm::value_ptr(value));
}

bool UBLightsBuffer::setBool(const char* key, const bool& value)
{
  return UniformBlockBuffer::set(mBufferId, mOffsets, key, sizeof(bool), &value);
}

bool UBLightsBuffer::setColor(const char* key, const Color& value)
{
  return UniformBlockBuffer::set(mBufferId, mOffsets, key, sizeof(Color), glm::value_ptr(value));
}

void UBLightsBuffer::getLightUniformNames(const std::string& prefix, std::vector<std::string>& uniformNames)
{
  uniformNames.reserve(uniformNames.size() + 4);
  uniformNames.push_back(prefix + ".color.ambient");
  uniformNames.push_back(prefix + ".color.diffuse");
  uniformNames.push_back(prefix + ".color.specular");
  uniformNames.push_back(prefix + ".off");
}

void UBLightsBuffer::getAttenuationUniformNames(
  const std::string& prefix, std::vector<std::string>& uniformNames
)
{
  uniformNames.reserve(uniformNames.size() + 3);
  uniformNames.push_back(prefix + ".constant");
  uniformNames.push_back(prefix + ".linear");
  uniformNames.push_back(prefix + ".quadratic");
}

void UBLightsBuffer::getDirLightUniformNames(
  const unsigned int numDirLights, std::vector<std::string>& uniformNames
)
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
)
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
)
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

void UBLightsBuffer::copyDirLights(const UBLights& lights, char* padded)
{
  for(unsigned int i = 0; i < lights.dirLights.size(); i++)
  {
    copyDirLight(i, lights.dirLights[i], padded);
  }
}

void UBLightsBuffer::copyPointLights(const UBLights& lights, char* padded)
{
  for(unsigned int i = 0; i < lights.pointLights.size(); i++)
  {
    copyPointLight(i, lights.pointLights[i], padded);
  }
}

void UBLightsBuffer::copySpotLights(const UBLights& lights, char* padded)
{
  for(unsigned int i = 0; i < lights.spotLights.size(); i++)
  {
    copySpotLight(i, lights.spotLights[i], padded);
  }
}

void UBLightsBuffer::copyLight(const std::string& prefix, Light light, char* padded)
{
  memcpy(&padded[mOffsets[prefix + ".color.ambient"]], glm::value_ptr(light.color.ambient), sizeof(Color));
  memcpy(&padded[mOffsets[prefix + ".color.diffuse"]], glm::value_ptr(light.color.diffuse), sizeof(Color));
  memcpy(&padded[mOffsets[prefix + ".color.specular"]], glm::value_ptr(light.color.specular), sizeof(Color));
  memcpy(&padded[mOffsets[prefix + ".off"]], &light.off, sizeof(bool));
}

void UBLightsBuffer::copyAttenuation(const std::string& prefix, Attenuation attenuation, char* padded)
{
  memcpy(&padded[mOffsets[prefix + ".constant"]], &attenuation.constant, sizeof(float));
  memcpy(&padded[mOffsets[prefix + ".linear"]], &attenuation.linear, sizeof(float));
  memcpy(&padded[mOffsets[prefix + ".quadratic"]], &attenuation.quadratic, sizeof(float));
}

void UBLightsBuffer::copyDirLight(const unsigned int index, const DirLight& dirLight, char* padded)
{
  const std::string prefix = "u_dir_lights[" + std::to_string(index) + "]";
  copyLight(prefix + ".light", dirLight.light, padded);
}

void UBLightsBuffer::copyPointLight(const unsigned int index, const PointLight& pointLight, char* padded)
{
  const std::string prefix = "u_point_lights[" + std::to_string(index) + "]";

  memcpy(&padded[mOffsets[prefix + ".position"]], glm::value_ptr(pointLight.position), sizeof(glm::vec3));
  copyAttenuation(prefix + ".attenuation", pointLight.attenuation, padded);
  copyLight(prefix + ".light", pointLight.light, padded);
}

void UBLightsBuffer::copySpotLight(const unsigned int index, const SpotLight& spotLight, char* padded)
{
  const std::string prefix = "u_spot_lights[" + std::to_string(index) + "]";

  memcpy(&padded[mOffsets[prefix + ".direction"]], glm::value_ptr(spotLight.direction), sizeof(glm::vec3));
  memcpy(&padded[mOffsets[prefix + ".cutOff"]], &spotLight.cutOff, sizeof(float));
  memcpy(&padded[mOffsets[prefix + ".outerCutOff"]], &spotLight.outerCutOff, sizeof(float));
  memcpy(&padded[mOffsets[prefix + ".position"]], glm::value_ptr(spotLight.position), sizeof(glm::vec3));

  copyAttenuation(prefix + ".attenuation", spotLight.attenuation, padded);
  copyLight(prefix + ".light", spotLight.light, padded);
}
