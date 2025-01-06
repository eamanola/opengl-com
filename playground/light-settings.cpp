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
    .dirLights = getDirLights(),
    .pointLights = getPointLights(),
    .spotLights = getSpotLights(),
  });
}

std::vector<DirLight> LightSettings::getDirLights()
{
  const glm::vec4 AMBIENT(0.2f);
  const glm::vec4 DIFFUSE(0.5f);
  const glm::vec4 SPECULAR(1.0f);
  const glm::vec4 color(1.0f);

  const glm::vec3 direction(-0.2f, -1.0f, -0.3f);
  const glm::vec4 ambient = color * AMBIENT;
  const glm::vec4 diffuse = color * DIFFUSE;
  const glm::vec4 specular = color * SPECULAR;
  const bool off = false;

  DirLight light {
    .direction = direction,
    .light = {
      .color = {
        .ambient = ambient,
        .diffuse = diffuse,
        .specular = specular,
      },
      .off = off,
    },
  };

  return { light };
}

std::vector<PointLight> LightSettings::getPointLights()
{
  const glm::vec4 AMBIENT(0.2f);
  const glm::vec4 DIFFUSE(0.5f);
  const glm::vec4 SPECULAR(1.0f);

  const float aConstant = 1.f;
  const float aLinear = 0.09f;
  const float aQuadratic = 0.032f;

  const bool off = false;

  std::vector<PointLight> pointLights;
  pointLights.reserve(mLights.positions.size());
  for (unsigned int i = 0; i < mLights.positions.size(); i++) {
    PointLight light = {
      .position = mLights.positions[i],
      .attenuation = {
        .constant = aConstant,
        .linear = aLinear,
        .quadratic = aQuadratic
      },
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

void LightSettings::updatePointLight0Position()
{
  ub_lights.setVec3("u_point_lights[0].position", mLights.positions[0]);
}

std::vector<SpotLight> LightSettings::getSpotLights()
{
  const glm::vec4 AMBIENT(0.2f);
  const glm::vec4 DIFFUSE(0.5f);
  const glm::vec4 SPECULAR(1.0f);
  const glm::vec4 color(1.0f);

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

  SpotLight spotLight = {
    .direction = direction,
    .cutOff = cutOff,
    .outerCutOff = outerCutOff,
    .position = position,
    .attenuation = { .constant = aConstant, .linear = aLinear, .quadratic = aQuadratic },
    .light = {
      .color = { .ambient = ambient, .diffuse = diffuse, .specular = specular },
      .off = off,
    },
  };

  return { spotLight };
}

void LightSettings::updateSpotLight(const glm::vec3& position, const glm::vec3& direction, bool off)
{
  ub_lights.setVec3("u_spot_lights[0].position", position);
  ub_lights.setVec3("u_spot_lights[0].direction", direction);
  ub_lights.setBool("u_spot_lights[0].light.off", off);
}
