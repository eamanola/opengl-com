#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/gl.h>

#include "shader.h"

Shader::Shader(const char* vPath, const char* fPath)
{
  std::string vSource;
  std::string fSource;

  try
  {
    vSource = readFile(vPath);
    fSource = readFile(fPath);
  }
  catch(std::ifstream::failure& e)
  {
    std::cout << "Error loading shader" << e.what() << std::endl;
    return;
  }
  const unsigned int vShader = compile(GL_VERTEX_SHADER, vSource.c_str());
  if (!vShader) {
    glDeleteShader(vShader);
    return;
  }

  const unsigned int fShader = compile(GL_FRAGMENT_SHADER, fSource.c_str());
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

void Shader::use(bool useThis)
{
  glUseProgram(useThis ? ID : 0);
}

void Shader::free()
{
  glDeleteProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4fv(const std::string &name, float* value) const
{
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setVec3(const std::string &name, float* value) const
{
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

const std::string Shader::readFile(const char* path)
{
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  file.open(path);

  std::stringstream stream;
  stream << file.rdbuf();

  file.close();

  return stream.str();
}

const unsigned int Shader::compile(GLenum type, const char* source)
{
  unsigned int shaderId = glCreateShader(type);
  glShaderSource(shaderId, 1, &source, nullptr);
  glCompileShader(shaderId);

  int success;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
  if (!success) {
    int maxLength = 1024;
	  // glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &maxLength);
    char infoLog[maxLength];
    glGetShaderInfoLog(shaderId, maxLength, nullptr, infoLog);

    std::cout << "Shader compilation failed ("
      << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << "): \n"
      << infoLog << std::endl;

    return 0;
  }

  return shaderId;
}
