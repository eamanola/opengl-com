#ifndef LIGHTED_SHADER
#define LIGHTED_SHADER

#include "plain-shader.h"

struct PhongColor
{
  Color ambient;
  Color diffuse;
  Color specular;
};

struct Light
{
  bool off;
  PhongColor color;
};

struct DirLight
{
  glm::vec3 direction;
  Light light;
};

struct Attenuation {
  float constant;
  float linear;
  float quadratic;
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

class LightedShader : public PlainShader
{
public:
  LightedShader(
    const unsigned int nrDirLights,
    const unsigned int nrPointLights,
    const unsigned int nrSpotLights,
    const char* vPath = "./shaders/lighting.vs",
    const char* fPath = "./shaders/lighting.fs",
    const std::vector<std::string>& prependTexts = {},
    const std::vector<std::string>& prependFiles = {}
  )
  :
  PlainShader(
    vPath,
    fPath,
    concat(
      {
        "#define IN_NR_DIR_LIGHTS " + std::to_string(nrDirLights) + "\n" +
        "#define IN_NR_POINT_LIGHTS " + std::to_string(nrPointLights) + "\n" +
        "#define IN_NR_SPOT_LIGHTS " + std::to_string(nrSpotLights) + "\n"
      },
      prependTexts
    ),
    concat(
      {
        "shaders/lighted-shader-defines"
      },
      prependFiles
    )
  ),
  NR_DIR_LIGHTS(nrDirLights),
  NR_POINT_LIGHTS(nrPointLights),
  NR_SPOT_LIGHTS(nrSpotLights)
  {}

  virtual ~LightedShader() {};

  const unsigned int NR_DIR_LIGHTS;
  const unsigned int NR_POINT_LIGHTS;
  const unsigned int NR_SPOT_LIGHTS;

  virtual void setColor(const Color& color) const override { setColor(PhongColor{ .diffuse = color }); }

  virtual void setViewPos(const glm::vec3& viewPos) const { setVec3fv("u_view_pos", viewPos); }

  virtual void setColor(const PhongColor& color) const
  {
    setVec4fv("u_material.diffuse_color", color.diffuse);
    setVec4fv("u_material.specular_color", color.specular);
  }

  virtual void setShininess(const float& shininess) const { setFloat("u_material.shininess", shininess); }
  virtual void setDirLight(unsigned int index, const DirLight& dirLight) const
  {
    const std::string key = "u_dir_lights[" + std::to_string(index) + "]";

    setVec3fv((key + ".direction").c_str(), dirLight.direction);
    setBool((key + ".light.off").c_str(), dirLight.light.off);
    setVec4fv((key + ".light.ambient").c_str(), dirLight.light.color.ambient);
    setVec4fv((key + ".light.diffuse").c_str(), dirLight.light.color.diffuse);
    setVec4fv((key + ".light.specular").c_str(), dirLight.light.color.specular);
  }

  virtual void setPointLight(unsigned int index, const PointLight& pointLight) const
  {
    const std::string key = "u_point_lights[" + std::to_string(index) + "]";

    setVec3fv((key + ".position").c_str(), pointLight.position);

    setFloat((key + ".attenuation.contant").c_str(), pointLight.attenuation.constant);
    setFloat((key + ".attenuation.linear").c_str(), pointLight.attenuation.linear);
    setFloat((key + ".attenuation.quadratic").c_str(), pointLight.attenuation.quadratic);

    setBool((key + ".light.off").c_str(), pointLight.light.off);
    setVec4fv((key + ".light.ambient").c_str(), pointLight.light.color.ambient);
    setVec4fv((key + ".light.diffuse").c_str(), pointLight.light.color.diffuse);
    setVec4fv((key + ".light.specular").c_str(), pointLight.light.color.specular);
  }

  virtual void setSpotLight(unsigned int index, const SpotLight& spotLight) const
  {
    const std::string key = "u_spot_lights[" + std::to_string(index) + "]";

    setVec3fv((key + ".direction").c_str(), spotLight.direction);
    setFloat((key + ".cutOff").c_str(), spotLight.cutOff);
    setFloat((key + ".outerCutOff").c_str(), spotLight.outerCutOff);
    setVec3fv((key + ".position").c_str(), spotLight.position);
    setFloat((key + ".attenuation.contant").c_str(), spotLight.attenuation.constant);
    setFloat((key + ".attenuation.linear").c_str(), spotLight.attenuation.linear);
    setFloat((key + ".attenuation.quadratic").c_str(), spotLight.attenuation.quadratic);
    setBool((key + ".light.off").c_str(), spotLight.light.off);
    setVec4fv((key + ".light.ambient").c_str(), spotLight.light.color.ambient);
    setVec4fv((key + ".light.diffuse").c_str(), spotLight.light.color.diffuse);
    setVec4fv((key + ".light.specular").c_str(), spotLight.light.color.specular);
  }

private:
  static std::vector<std::string> concat(
    const std::vector<std::string>& a, const std::vector<std::string>& b
  ) {
    std::vector<std::string> ret = std::vector<std::string>(a);
    ret.insert(ret.end(), b.begin(), b.end());
    return ret;
  }
};

#endif
