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
    std::cout << "Vertex shader compilation failed";
    glDeleteShader(vShader);
    return;
  }

  const unsigned int fShader = compile(GL_FRAGMENT_SHADER, fSource.c_str());
  if (!fShader) {
    std::cout << "Fragment shader compilation failed";

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
    int maxLength;
	  glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &maxLength);
    char infoLog[maxLength];
    glGetProgramInfoLog(ID, maxLength, nullptr, infoLog);
    std::cout << "Linking failed" << infoLog << std::endl;

    glDeleteProgram(ID);
    return;
  }
}

void Shader::use()
{
  glUseProgram(ID);
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
    int maxLength;
	  glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &maxLength);
    char infoLog[maxLength];
    glGetShaderInfoLog(shaderId, maxLength, nullptr, infoLog);

    std::cout << "Shader compilation failed\n"
      << (type == GL_VERTEX_SHADER ? "Vertex" : "fragment") << "\n"
      << infoLog << std::endl;

    return 0;
  }

  return shaderId;
}
