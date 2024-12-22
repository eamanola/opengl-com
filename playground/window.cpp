#include "window.h"
#include "../vertex.h"
#include "../shader-utils.h"
#include <glm/gtc/matrix_transform.hpp>

Window::Window(/* args */)
:
mMesh(
  std::vector<Vertex> ({
    { .position = { -0.5f, -0.5f, 0.f }, .normal = { 0.f, 0.f, 1.f }, .texCoords = { 0.f, 0.f } },
    { .position = {  0.5f, -0.5f, 0.f }, .normal = { 0.f, 0.f, 1.f }, .texCoords = { 1.f, 0.f } },
    { .position = {  0.5f,  0.5f, 0.f }, .normal = { 0.f, 0.f, 1.f }, .texCoords = { 1.f, 1.f } },
    { .position = { -0.5f,  0.5f, 0.f }, .normal = { 0.f, 0.f, 1.f }, .texCoords = { 0.f, 1.f } }
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
  glDisable(GL_CULL_FACE);

  glm::mat4 model;

  glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.55f));
    shader.setMat4fv("u_model", model);
    mMesh.draw(shader, &mTexture);

    // order matters / depth buffer
    // 2nd will glitch through last window
    // semi transpaers shoudl draw last, and in order from furthest to nearest
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.4f, 1.2f, 0.6f));
    shader.setMat4fv("u_model", model);
    mMesh.draw(shader, &mTexture);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.6f, 0.8f, 0.555f));
    shader.setMat4fv("u_model", model);
    mMesh.draw(shader, &mTexture);
  glDisable(GL_BLEND);

  glEnable(GL_CULL_FACE);
}

void Window::free()
{
  mMesh.free();
  glDeleteTextures(1, &mTexture.id);
}
