#include "lighting-settings.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

LightingSettings::LightingSettings()
{
}

LightingSettings::~LightingSettings()
{
}

void LightingSettings::setup(const LightedShader& shader)
{
  shader.setShininess(32.f);

  initDirLight(shader);
  initSpotLights(shader);
}

void LightingSettings::initDirLight(const LightedShader& shader)
{
  const Color AMBIENT (0.2f, 0.2f, 0.2f, 0.2f);
  const Color DIFFUSE (0.5f, 0.5f, 0.5f, 0.5f);
  const Color SPECULAR(1.0f, 1.0f, 1.0f, 1.0f);
  const bool off = false;

  const Color color(0.2f, 0.2f, 0.2f, 0.2f);
  const glm::vec3 direction(0.f, -1.0f, 1.f);

  DirLight dirLight
  {
    .direction = direction,
    .light = Light
    {
      .off = off,
      .color = PhongColor
      {
        .ambient = color * AMBIENT,
        .diffuse = color * DIFFUSE,
        .specular = color * SPECULAR
      }
    }
  };

  shader.setDirLight(0, dirLight);
}

void LightingSettings::initFloorLights(const LightedShader& shader, const Floor& floor)
{
  const bool off = false;

  // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
  // const Attenuation attenuation = { 1.0, 0.0014, 0.000007 }; // 3250
  // const Attenuation attenuation = { 1.0, 0.007,  0.0002   }; // 600
  // const Attenuation attenuation = { 1.0, 0.014,  0.0007   }; // 325
  // const Attenuation attenuation = { 1.0, 0.022,  0.0019   }; // 200
  // const Attenuation attenuation = { 1.0, 0.027,  0.0028   }; // 160
  // const Attenuation attenuation = { 1.0, 0.045,  0.0075   }; // 100
  // const Attenuation attenuation = { 1.0, 0.07,   0.017    }; // 65
  // const Attenuation attenuation = { 1.0, 0.09,   0.032    }; // 50
  // const Attenuation attenuation = { 1.0, 0.14,   0.07     }; // 32
  // const Attenuation attenuation = { 1.0, 0.22,   0.20     }; // 20
  // const Attenuation attenuation = { 1.0, 0.35,   0.44     }; // 13
  const Attenuation attenuation = { 1.0, 0.7,    1.8      }; // 7
  const std::vector<glm::vec3> positions = floor.positions();

  const glm::mat4 model = floor.model();

  for(unsigned int i = 0; i < positions.size(); i++)
  {
    PointLight pointLight = {
      .position = glm::vec3(glm::translate(model, positions[i] + glm::vec3(0.f, 0.f, -0.2f))[3]),
      .attenuation = attenuation,
      .light = Light {
        .off = off,
      }
    };

    shader.setPointLight(i, pointLight);
  }

  updateFloorLights(shader, floor);
}

void LightingSettings::updateFloorLights(const Shader& shader, const Floor& floor)
{
  const Color AMBIENT (0.2f, 0.2f, 0.2f, 0.2f);
  const Color DIFFUSE (0.5f, 0.5f, 0.5f, 0.5f);
  const Color SPECULAR(1.0f, 1.0f, 1.0f, 1.f);

  const std::vector<Color> colors = floor.colors();

  for(unsigned int i = 0; i < colors.size(); i++)
  {
    Color color(colors[i]);
    color.a = 1.f;

    std::stringstream key;
    key << "u_point_lights[" << i << "]";
    shader.setVec4fv(key.str() + ".light.ambient", color * AMBIENT);
    shader.setVec4fv(key.str() + ".light.diffuse", color * DIFFUSE);
    shader.setVec4fv(key.str() + ".light.specular", color * SPECULAR);
  }
}

void LightingSettings::initSpotLights(const LightedShader& shader)
{
  const Color AMBIENT (0.2f, 0.2f, 0.2f, 0.2f);
  const Color DIFFUSE (0.5f, 0.5f, 0.5f, 0.5f);
  const Color SPECULAR(1.0f, 1.0f, 1.0f, 1.f);
  const bool off = false;

  // const Attenuation attenuation = { 1.0, 0.0014, 0.000007 }; // 3250
  // const Attenuation attenuation = { 1.0, 0.007,  0.0002   }; // 600
  // const Attenuation attenuation = { 1.0, 0.014,  0.0007   }; // 325
  // const Attenuation attenuation = { 1.0, 0.022,  0.0019   }; // 200
  // const Attenuation attenuation = { 1.0, 0.027,  0.0028   }; // 160
  const Attenuation attenuation = { 1.0, 0.045,  0.0075   }; // 100
  // const Attenuation attenuation = { 1.0, 0.07,   0.017    }; // 65
  // const Attenuation attenuation = { 1.0, 0.09,   0.032    }; // 50
  // const Attenuation attenuation = { 1.0, 0.14,   0.07     }; // 32
  // const Attenuation attenuation = { 1.0, 0.22,   0.20     }; // 20
  // const Attenuation attenuation = { 1.0, 0.35,   0.44     }; // 13
  // const Attenuation attenuation = { 1.0, 0.7,    1.8      }; // 7
  const std::vector<glm::vec3>& positions = mSpotLights.positions;
  const std::vector<Color>& colors = mSpotLights.colors;

  const float cutOff = glm::cos(glm::radians(10.5f));
  const float outerCutOff = glm::cos(glm::radians(12.5f));
  const std::vector<glm::vec3>& directions = mSpotLights.directions;

  for(unsigned int i = 0; i < NR_SPOT_LIGHTS; i++)
  {
    SpotLight spotLight = {
      .direction = directions[i],
      .cutOff = cutOff,
      .outerCutOff = outerCutOff,
      .position = positions[i],
      .attenuation = attenuation,
      .light = Light {
        .off = off,
        .color = PhongColor {
          .ambient = colors[i] * AMBIENT,
          .diffuse = colors[i] * DIFFUSE,
          .specular = colors[i] * SPECULAR
        }
      }
    };

    shader.setSpotLight(i, spotLight);
  }
}

void LightingSettings::updateSpotLights(Shader &shader)
{
  for(unsigned int i = 0; i < NR_SPOT_LIGHTS; i++)
  {
    std::stringstream key;
    key << "u_spot_lights[" << i << "]";

    shader.setVec3fv(key.str() + ".direction", mSpotLights.directions[i]);
  }
}
