#include "lighting.h"

#include <glm/gtc/type_ptr.hpp>

#define DIRECTIONAL_LIGHT
#define SPOT_LIGHT
#define POINT_LIGHT

Lighting::Lighting(
  unsigned int bindingId,
  const std::vector<Shader>& shaders,
  unsigned int numDirLights,
  unsigned numPointLights,
  unsigned numSpotLights
) :
  ubLights(bindingId, shaders, numDirLights, numPointLights, numSpotLights)
{
  ub_lights initValue {
    .dirLights = getDirLights(),
    .pointLights = getPointLights(),
    .spotLights = getSpotLights(),
  };
  ubLights.set(initValue);
}

void Lighting::setup(Shader& shader) { }

std::vector<DirLight> Lighting::getDirLights()
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

std::vector<PointLight> Lighting::getPointLights()
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

void Lighting::updatePointLight0Position()
{
  ubLights.setVec3("u_point_lights[0].position", mLights.positions[0]);
}

std::vector<SpotLight> Lighting::getSpotLights()
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

void Lighting::updateSpotLight(const glm::vec3& position, const glm::vec3& direction, bool off)
{
  ubLights.setVec3("u_spot_lights[0].position", position);
  ubLights.setVec3("u_spot_lights[0].direction", direction);
  ubLights.setBool("u_spot_lights[0].light.off", off);
}
