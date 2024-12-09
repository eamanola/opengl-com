#include "shader-utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stb_image.h>

const unsigned int ShaderUtils::compileShader(GLenum type, const char* path)
{
  std::string source;

  try
  {
    source = ShaderUtils::readFile(path);
  }
  catch(std::ifstream::failure& e)
  {
    std::cout << "Error loading shader" << e.what() << std::endl;
    return 0;
  }

  return ShaderUtils::compile(type, source.c_str());
}

unsigned int ShaderUtils::loadTexture(std::string path, std::string directory)
{
  unsigned int texture;
  glGenTextures(1, &texture);

  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannel;
  unsigned char* data = stbi_load((directory + '/' + path).c_str(), &width, &height, &nrChannel, 0);
  if (data) {
    GLenum format;

    if(nrChannel == 3) format = GL_RGB;
    else if (nrChannel == 4) format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
    glDeleteTextures(1, &texture);
    texture = 0;
  }

  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

const std::string ShaderUtils::readFile(const char* path)
{
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  file.open(path);

  std::stringstream stream;
  stream << file.rdbuf();

  file.close();

  return stream.str();
}

const unsigned int ShaderUtils::compile(GLenum type, const char* source)
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

