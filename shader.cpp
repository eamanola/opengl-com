#include <iostream>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "shader-utils/shader-utils.h"

Shader::Shader(const char* vPath, const char* fPath, const std::vector<std::string>& defines)
{
  const unsigned int vShader = ShaderUtils::compileShader(GL_VERTEX_SHADER, vPath, defines);
  if (!vShader) {
    glDeleteShader(vShader);
    return;
  }

  const unsigned int fShader = ShaderUtils::compileShader(GL_FRAGMENT_SHADER, fPath, defines);
  if (!fShader) {
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return;
  }

  ID = glCreateProgram();
  glAttachShader(ID, vShader);
  glAttachShader(ID, fShader);
  glLinkProgram(ID);

  glDeleteShader(vShader);
  glDeleteShader(fShader);

  int success;
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if(!success)
  {
    int maxLength = 1024;
	  // glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &maxLength);
    char infoLog[maxLength];
    glGetProgramInfoLog(ID, maxLength, nullptr, infoLog);
    std::cout << "Linking failed:\n" << infoLog << std::endl;

    glDeleteProgram(ID);
    return;
  }
}

void Shader::use() const
{
  glUseProgram(ID);
}

void Shader::free() const
{
  glDeleteProgram(ID);
}

void Shader::setBool(const std::string &name, const bool &value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, const int &value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, const float &value) const
{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4fv(const std::string &name, const glm::mat4 &value) const
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3fv(const std::string &name, const glm::vec3 &value) const
{
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, (float*)glm::value_ptr(value));
}

void Shader::setVec4fv(const std::string &name, const glm::vec4 &value) const
{
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, (float*)glm::value_ptr(value));
}
