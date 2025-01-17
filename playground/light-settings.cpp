#include "light-settings.h"

#include <glm/gtc/type_ptr.hpp>

using Lighting::DirLight;
using Lighting::PointLight;
using Lighting::SpotLight;

LightSettings::LightSettings(
  unsigned int bindingId,
  const std::vector<Shader>& shaders,
  unsigned int numDirLights,
  unsigned numPointLights,
  unsigned numSpotLights
) :
  ub_lights(bindingId, shaders, numDirLights, numPointLights, numSpotLights)
{
  ub_lights.set({
    .dirLights = getDirLights(numDirLights),
    .pointLights = getPointLights(numPointLights),
    .spotLights = getSpotLights(numSpotLights),
  });
}

std::vector<DirLight> LightSettings::getDirLights(unsigned int count) const
{
  return std::vector<DirLight>(
    mDirLights.begin(), mDirLights.begin() + (std::min(count, (unsigned int)mDirLights.size()))
  );
}

std::vector<PointLight> LightSettings::getPointLights(unsigned int count) const
{
  const Color AMBIENT(0.2f);
  const Color DIFFUSE(0.5f);
  const Color SPECULAR(1.0f);

  const bool off = false;

  std::vector<PointLight> pointLights;
  unsigned int size = std::min(count, (unsigned int)mLights.positions.size());
  pointLights.reserve(size);
  for (unsigned int i = 0; i < size; i++) {
    PointLight light = {
      .position = mLights.positions[i],
      .attenuation = mLights.attenuations[i],
      .light = {
        .color = {
          .ambient = mLights.colors[i] * AMBIENT,
          .diffuse = mLights.colors[i] * DIFFUSE,
          .specular = mLights.colors[i] * SPECULAR,
        },
        .off = off
      }
    };
    pointLights.push_back(light);
  }

  return pointLights;
}

void LightSettings::updatePointLight0Position() const
{
  ub_lights.setVec3("u_point_lights[0].position", mLights.positions[0]);
}

void LightSettings::setPointLight0Position(const glm::vec2& xy)
{
  mLights.positions[0] = glm::vec3(xy, mLights.positions[0].z);
}

std::vector<SpotLight> LightSettings::getSpotLights(unsigned int count)
{
  if (count == 0)
    return {};
  const glm::vec4 AMBIENT(0.2f);
  const glm::vec4 DIFFUSE(0.5f);
  const glm::vec4 SPECULAR(1.0f);
  const glm::vec4 color(1.0f);

  const glm::vec3 direction = glm::vec3(0.0f);
  const float cutOff = glm::cos(glm::radians(12.5f));
  const float outerCutOff = glm::cos(glm::radians(17.5f));
  const glm::vec3 position = glm::vec3(0.f);

  const bool off = true;
  const glm::vec4 ambient = color * AMBIENT;
  const glm::vec4 diffuse = color * DIFFUSE;
  const glm::vec4 specular = color * SPECULAR;

  SpotLight spotLight = {
    .direction = direction,
    .cutOff = cutOff,
    .outerCutOff = outerCutOff,
    .position = position,
    .attenuation = ATTENUATION_50,
    .light = {
      .color = { .ambient = ambient, .diffuse = diffuse, .specular = specular },
      .off = off,
    },
  };

  return { spotLight };
}

void LightSettings::updateSpotLight(const glm::vec3& position, const glm::vec3& direction, bool off)
  const
{
  ub_lights.setVec3("u_spot_lights[0].position", position);
  ub_lights.setVec3("u_spot_lights[0].direction", direction);
  ub_lights.setBool("u_spot_lights[0].light.off", off);
}
