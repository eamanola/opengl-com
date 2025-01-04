#include "gl-utils.h"
#include <iostream>

const unsigned int GLUtils::compileShader(GLenum type, const char* source)
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

void GLUtils::deleteTextures(const unsigned int lenght, const unsigned int* textureIds)
{
  glDeleteTextures(lenght, textureIds);
}
