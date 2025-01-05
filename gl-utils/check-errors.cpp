#include "gl-utils.h"

#include <iostream>

std::vector<int> GLUtils::checkErrors()
{
  int error;
  std::vector<int> errors;

  while ((error = glGetError()) != GL_NO_ERROR)
  {
    errors.push_back(error);
    std::cerr << "GLError: " << error << "\n";
  }

  return errors;
}

bool GLUtils::noErrors()
{
  return GLUtils::checkErrors().size() == 0;
}
