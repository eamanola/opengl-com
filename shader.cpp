#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "gl-utils/gl-utils.h"
#include "shader.h"
#include "utils/utils.h"

Shader::Shader(
  const char* vPath,
  const char* gPath,
  const char* fPath,
  const std::vector<std::string>& prependTexts,
  const std::vector<std::string>& prependFiles
)
{
  unsigned int vShader;
  if (!GLUtils::compileShader(
        GL_VERTEX_SHADER, Utils::shaderSource(vPath, prependTexts, prependFiles).c_str(), vShader
      )) {
    std::cout << "Failed to compile vertex shader\n";
    std::cout << vPath << "\n";
    return;
  }

  unsigned int fShader;
  if (!GLUtils::compileShader(
        GL_FRAGMENT_SHADER, Utils::shaderSource(fPath, prependTexts, prependFiles).c_str(), fShader
      )) {
    std::cout << "Failed to compile fragment shader\n";
    std::cout << fPath << "\n";
    return;
  }

  unsigned int gShader = 0;
  if (gPath != nullptr) {
    if (!GLUtils::compileShader(
          GL_GEOMETRY_SHADER,
          Utils::shaderSource(gPath, prependTexts, prependFiles).c_str(),
          gShader
        )) {
      std::cout << "Failed to compile fragment shader\n";
      std::cout << gPath << "\n";
      return;
    }
  }

  ID = glCreateProgram();
  glAttachShader(ID, vShader);
  glAttachShader(ID, fShader);
  if (gShader != 0)
    glAttachShader(ID, gShader);
  glLinkProgram(ID);

  glDeleteShader(vShader);
  glDeleteShader(fShader);
  if (gShader != 0)
    glDeleteShader(gShader);

  int success;
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    int maxLength = 1024;
    // glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &maxLength);
    char infoLog[maxLength];
    glGetProgramInfoLog(ID, maxLength, nullptr, infoLog);
    std::cout << "Linking failed:\n" << infoLog << "\n";
    std::cout << vPath << "\n";
    if (gPath != nullptr)
      std::cout << gPath << "\n";
    std::cout << fPath << "\n";
    glDeleteProgram(ID);
    return;
  }
}

void Shader::use() const { glUseProgram(ID); }

void Shader::free() const { glDeleteProgram(ID); }

void Shader::setBool(const char* name, const bool& value) const { setInt(name, (int)value); }

void Shader::setInt(const char* name, const int& value) const
{
  glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::setFloat(const char* name, const float& value) const
{
  glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::setMat3fv(const char* name, const glm::mat3& value) const
{
  glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4fv(const char* name, const glm::mat4& value) const
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3fv(const char* name, const glm::vec3& value) const
{
  glUniform3fv(glGetUniformLocation(ID, name), 1, (float*)glm::value_ptr(value));
}

void Shader::setVec4fv(const char* name, const glm::vec4& value) const
{
  glUniform4fv(glGetUniformLocation(ID, name), 1, (float*)glm::value_ptr(value));
}
