#include "window.h"
#include "../vertex.h"
#include "../shader-utils.h"
#include "glm/gtc/matrix_transform.hpp"

Window::Window(/* args */)
:
mMesh(
  std::vector<Vertex> ({
    {
      .position  = glm::vec3(-0.5f, -0.5f, 0.f),
      .normal = glm::vec3(0.f, 0.f, 1.f),
      .texCoords = glm::vec2(0.f, 0.f)
    },
    {
      .position = glm::vec3(0.5f, -0.5f, 0.f),
      .normal = glm::vec3(0.f, 0.f, 1.f),
      .texCoords = glm::vec2(1.f, 0.f)
    },
    {
      .position = glm::vec3(0.5f, 0.5f, 0.f),
      .normal = glm::vec3(0.f, 0.f, 1.f),
      .texCoords = glm::vec2(1.f, 1.f)
    },
    {
      .position = glm::vec3(-0.5f,  0.5f, 0.f),
      .normal = glm::vec3(0.f, 0.f, 1.f),
      .texCoords = glm::vec2(0.f, 1.f)
    }
  }),
  std::vector<unsigned int> ({
    0, 1, 2,
    0, 2, 3
  })
)
{
  const char * path = "assets/blending_transparent_window.png";

  mTexture = Texture
  {
    .id = ShaderUtils::loadTexture(path),
    .type = TEXTURE_TYPE_DIFFUSE,
    .path = path
  };
}

Window::~Window()
{
}

void Window::draw(Shader& shader)
{
  const std::vector<Texture> texture = { mTexture };
  glm::mat4 model;
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.55f));
    shader.setMat4fv("u_model", model);
    mMesh.draw(shader, texture);

    // order matters / depth buffer
    // 2nd will glitch through last window
    // semi transpaers shoudl draw last, and in order from furthest to nearest
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.4f, 1.2f, 0.6f));
    shader.setMat4fv("u_model", model);
    mMesh.draw(shader, texture);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.6f, 0.8f, 0.555f));
    shader.setMat4fv("u_model", model);
    mMesh.draw(shader, texture);
  glDisable(GL_BLEND);
}

void Window::free()
{
  mMesh.free();
  glDeleteTextures(1, &mTexture.id);
}
