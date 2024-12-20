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

unsigned int ShaderUtils::loadTexture(
  const char* path,
  const GLint wrap
)
{
  unsigned int textureId = 0;

  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannel;
  unsigned char* data = stbi_load(path, &width, &height, &nrChannel, 0);
  if (data) {
    textureId = ShaderUtils::createTexture(width, height, nrChannel, data, wrap);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);

  return textureId;
}

unsigned int ShaderUtils::loadTexture(
  const unsigned char* const buffer, const unsigned int len,
  const GLint wrap
)
{
  unsigned int textureId = 0;

  stbi_set_flip_vertically_on_load(true);
  int width, height, nrChannel;
  unsigned char* data = stbi_load_from_memory(buffer, len, &width, &height, &nrChannel, 0);
  if (data) {
    textureId = ShaderUtils::createTexture(width, height, nrChannel, data, wrap);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);

  return textureId;
}

const unsigned int ShaderUtils::createTexture(
  int width, int height, int nrChannel, unsigned char* data,
  const GLint wrap
)
{
  unsigned int textureId = 0;

  GLenum format;

  if(nrChannel == 3) format = GL_RGB;
  else if (nrChannel == 4) format = GL_RGBA;

  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  return textureId;
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

