#include "gl-utils.h"
#include <iostream>

bool GLUtils::compileShader(const GLenum type, const char* source, unsigned int& shaderId)
{
  shaderId = glCreateShader(type);
  glShaderSource(shaderId, 1, &source, nullptr);
  glCompileShader(shaderId);

  int success;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
  if (!success) {
    int maxLength = 1024;
	  // glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &maxLength);
    char infoLog[maxLength];
    glGetShaderInfoLog(shaderId, maxLength, nullptr, infoLog);

    std::cout << infoLog << "\n";

    glDeleteShader(shaderId);
    shaderId = 0;
    // GLUtils::checkErrors(); // flush errors?
    return false;
  }

  return GLUtils::noErrors();
}
