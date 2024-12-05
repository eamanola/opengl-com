#include "shape.h"

#include <glad/gl.h>
#include <iostream>

#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

Shape::Shape(const char* vShader, const char* fShader) : Shader(vShader, fShader)
{
}

Shape::~Shape()
{
}

void Shape::setModel(glm::mat4 model) const
{
  setMat4fv("model", glm::value_ptr(model));
}

void Shape::setView(glm::mat4 view) const
{
  setMat4fv("view", glm::value_ptr(view));
}

void Shape::setProjection(glm::mat4 projection) const
{
  setMat4fv("projection", glm::value_ptr(projection));
}

void Shape::free()
{
  Shader::free();
}

unsigned int Shape::loadTexture(std::string path, std::string directory)
{
  unsigned int texture;
  glGenTextures(1, &texture);

  int width, height, nrChannel;
  stbi_set_flip_vertically_on_load(true);
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
