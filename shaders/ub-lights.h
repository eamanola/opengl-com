#ifndef UB_LIGHTS_BUFFER_H
#define UB_LIGHTS_BUFFER_H

#include "uniform-block-buffer.h"

#include "attenuation.h"
#include "color.h"
#include "shader.h"

namespace Lighting
{

struct Light {
  PhongColor color;
  bool off;
};

struct DirLight {
  glm::vec3 direction;
  Light light;
};

struct PointLight {
  glm::vec3 position;
  Attenuation attenuation;

  Light light;
};

struct SpotLight {
  glm::vec3 direction;
  float cutOff;
  float outerCutOff;

  glm::vec3 position;
  Attenuation attenuation;

  Light light;
};

class ub_lights : public UniformBlockBuffer
{
public:
  struct Lights {
    std::vector<DirLight> dirLights;
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;
  };

  ub_lights(
    const unsigned int bindingId,
    const std::vector<Shader>& shaders,
    const unsigned int numDirLights,
    const unsigned int numPointLights,
    const unsigned int numSpotLights
  );
  ~ub_lights() { }

  bool set(const Lights& lights) const;
  bool setVec3(const char* uniformName, const glm::vec3& value) const;
  bool setColor(const char* uniformName, const Color& value) const;
  bool setBool(const char* uniformName, const bool& value) const;

private:
  std::vector<std::string> getUniformNames(
    unsigned int numDirLights, unsigned int numPointLights, unsigned int numSpotLights
  ) const;
  void
  getLightUniformNames(const std::string& prefix, std::vector<std::string>& uniformNames) const;
  void getAttenuationUniformNames(const std::string& prefix, std::vector<std::string>& uniformNames)
    const;
  void getDirLightUniformNames(
    const unsigned int numDirLights, std::vector<std::string>& uniformNames
  ) const;
  void getPointLightUniformNames(
    const unsigned int numPointLights, std::vector<std::string>& uniformNames
  ) const;
  void getSpotLightUniformNames(
    const unsigned int numSpotLights, std::vector<std::string>& uniformNames
  ) const;

  void copyDirLights(const Lights& lights, char* padded) const;
  void copyPointLights(const Lights& lights, char* padded) const;
  void copySpotLights(const Lights& lights, char* padded) const;

  void copyLight(const std::string& prefix, Light light, char* padded) const;
  void copyAttenuation(const std::string& prefix, Attenuation attenuation, char* padded) const;
  void copyDirLight(const unsigned int index, const DirLight& light, char* padded) const;
  void copyPointLight(const unsigned int index, const PointLight& light, char* padded) const;
  void copySpotLight(const unsigned int index, const SpotLight& light, char* padded) const;
};

} // namespace Lighting
#endif
