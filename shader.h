#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Shader
{
public:
  Shader(
    const char* vertexPath,
    const char* geometryPath,
    const char* fragmentPath,
    const std::vector<std::string>& prependTexts = {},
    const std::vector<std::string>& prependFiles = {}
  );

  void use() const;
  void free() const;
  void setFloat(const char* name, const float& value) const;
  void setMat3fv(const char* name, const glm::mat3& value) const;
  void setMat4fv(const char* name, const glm::mat4& value) const;
  void setVec3fv(const char* name, const glm::vec3& value) const;
  void setVec4fv(const char* name, const glm::vec4& value) const;
  void setBool(const char* name, const bool& value) const { setInt(name, (int)value); }
  void setSampler(const char* name, const int& offset) const { setInt(name, (int)offset); }
  const unsigned int id() const { return ID; }

private:
  void setInt(const char* name, const int& value) const;
  unsigned int ID;
};
#endif
