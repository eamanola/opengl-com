#include "gl-utils.h"

#include <iostream>

const std::string GLUtils_errorString(GLenum error)
{
  std::string str;

  switch (error) {
  case GL_NO_ERROR:
    str = "GL_NO_ERROR";
    break;

  case GL_INVALID_ENUM:
    str = "GL_INVALID_ENUM";
    break;

  case GL_INVALID_VALUE:
    str = "GL_INVALID_VALUE";
    break;

  case GL_INVALID_OPERATION:
    str = "GL_INVALID_OPERATION";
    break;

  case GL_STACK_OVERFLOW:
    str = "GL_STACK_OVERFLOW";
    break;

  case GL_STACK_UNDERFLOW:
    str = "GL_STACK_UNDERFLOW";
    break;

  case GL_OUT_OF_MEMORY:
    str = "GL_OUT_OF_MEMORY";
    break;

  case GL_INVALID_FRAMEBUFFER_OPERATION:
    str = "GL_INVALID_FRAMEBUFFER_OPERATION";
    break;

  default:
    str = std::to_string(error);
    break;
  }

  return str;
}

std::vector<unsigned int> GLUtils::checkErrors()
{
  GLenum error;
  std::vector<unsigned int> errors;

  while ((error = glGetError()) != GL_NO_ERROR) {
    errors.push_back(error);
    std::cerr << "GLError: " << GLUtils_errorString(error) << "\n";
  }

  return errors;
}

bool GLUtils::noErrors() { return GLUtils::checkErrors().size() == 0; }
