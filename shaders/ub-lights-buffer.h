#ifndef UB_LIGHTS_BUFFER_H
#define UB_LIGHTS_BUFFER_H

#include "../shader-utils/uniform-block.h"
#include "../shader-utils/uniform-block-buffer.h"
#include "../color.h"
#include "../shader.h"
#include <unordered_map>

struct PhongColor {
  Color ambient;
  Color diffuse;
  Color specular;
};

struct Light {
  PhongColor color;
  bool off;
};

struct Attenuation {
  float constant;
  float linear;
  float quadratic;
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

struct UBLights
{
  std::vector<DirLight> dirLights;
  std::vector<PointLight> pointLights;
  std::vector<SpotLight> spotLights;
};

class UBLightsBuffer
{
public:
  UBLightsBuffer(
    const std::vector<Shader>& shaders,
    const unsigned int bindingId,
    const unsigned int numDirLights,
    const unsigned int numPointLights,
    const unsigned int numSpotLights
  );
  ~UBLightsBuffer() {};

  void free() const { glDeleteBuffers(1, &mBufferId); }

  bool set(const UBLights& lights);
  bool setVec3(const char* key, const glm::vec3& value);
  bool setColor(const char* key, const Color& value);
  bool setBool(const char* key, const bool& value);

private:
  void getLightUniformNames(const std::string& prefix, std::vector<std::string>& uniformNames);
  void getAttenuationUniformNames(const std::string& prefix, std::vector<std::string>& uniformNames);
  void getDirLightUniformNames(const unsigned int numDirLights, std::vector<std::string>& uniformNames);
  void getPointLightUniformNames(const unsigned int numPointLights, std::vector<std::string>& uniformNames);
  void getSpotLightUniformNames(const unsigned int numSpotLights, std::vector<std::string>& uniformNames);

  void copyDirLights(const UBLights& lights, char* padded);
  void copyPointLights(const UBLights& lights, char* padded);
  void copySpotLights(const UBLights& lights, char* padded);

  void copyLight(const std::string& prefix, Light light, char* padded);
  void copyAttenuation(const std::string& prefix, Attenuation attenuation, char* padded);
  void copyDirLight(const unsigned int index, const DirLight& light, char* padded);
  void copyPointLight(const unsigned int index, const PointLight& light, char* padded);
  void copySpotLight(const unsigned int index, const SpotLight& light, char* padded);

  std::unordered_map<std::string, unsigned int> mOffsets;
  unsigned int mBufferId;
  int mBlockDataSize;
};

#endif
