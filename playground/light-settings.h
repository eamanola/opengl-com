#ifndef LIGHTING_SHADER_H
#define LIGHTING_SHADER_H

#include "shader.h"
#include "shaders/ub-lights.h"
#include <glm/glm.hpp>
#include <vector>

using Lighting::DirLight;
using Lighting::PointLight;
using Lighting::SpotLight;

class LightSettings
{
public:
  LightSettings(
    unsigned int bindingId,
    const std::vector<Shader>& shaders,
    unsigned int numDirLights,
    unsigned numPointLights,
    unsigned numSpotLights
  );
  ~LightSettings() { }

  // const unsigned int NR_POINT_LIGHTS = mLights.positions.size();

  void setup(const Shader& shader) { }
  void setPointLight0Position(const glm::vec2& xy);
  void updatePointLight0Position() const;
  void updateSpotLight(const glm::vec3& position, const glm::vec3& direction, bool off) const;
  void free() { ub_lights.free(); }
  std::vector<DirLight> getDirLights(unsigned int count) const;
  std::vector<PointLight> getPointLights(unsigned int count) const;

private:
  std::vector<SpotLight> getSpotLights(unsigned int count);
  Lighting::ub_lights ub_lights;
  std::vector<DirLight> mDirLights = {
    DirLight {
      .direction = glm::vec3(0.5f, -1.f, 0.25f),
      .light = {
        .color = PhongColor {
          .ambient = Color(1.f) * Color(0.2f),
          .diffuse = Color(1.f) * Color(0.5f),
          .specular = Color(1.f) * Color(1.f),
        },
        .off = false,
      },
    },
    DirLight {
      .direction = glm::vec3(-0.5f, -1.f, 0.25f),
      .light = {
        .color = PhongColor {
          .ambient = Color(1.f) * Color(0.2f),
          .diffuse = Color(1.f) * Color(0.5f),
          .specular = Color(1.f) * Color(1.f),
        },
        .off = false,
      },
    },
  };

  struct {
    std::vector<glm::vec3> positions = {
      glm::vec3(0.7f, 0.2f, 2.0f),
      glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f, 2.0f, -12.0f),
      glm::vec3(0.0f, 1.0f, -2.0f),
    };
    std::vector<Color> colors = {
      Color(1.0f, 1.0f, 1.0f, 1.0f),
      Color(10.0f, 0.0f, 0.0f, 1.0f),
      Color(0.0f, 10.0f, 0.0f, 1.0f),
      Color(0.0f, 0.0f, 10.0f, 1.0f),
    };
    std::vector<Attenuation> attenuations = {
      ATTENUATION_32,
      ATTENUATION_20,
      ATTENUATION_20,
      ATTENUATION_20,
    };
  } mLights;
};

#endif
