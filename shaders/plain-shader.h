#ifndef PLAIN_SHADER
#define PLAIN_SHADER

#include "../shader.h"
#include "../color.h"
#include <glm/glm.hpp>

class PlainShader : public Shader
{
public:
  PlainShader(
    const char* vPath = "./shaders/plain.vs",
    const char* fPath = "./shaders/single-color.fs",
    const std::vector<std::string>& defines = {}
  )
  : Shader(vPath, fPath, defines) {}
  virtual ~PlainShader() {};

  virtual void setModel(const glm::mat4& model) { setMat4fv("u_model", model ); }
  virtual void setProjXView(const glm::mat4& proj_x_view) { setMat4fv("u_proj_x_view", proj_x_view); }
  virtual void setColor(const Color& color) { setVec4fv("u_color", color); }
};

#endif
