#include "mirror.h"

#include "shaders/u-material.h"
#include "shaders/u-model.h"
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>

Mirror::Mirror(const unsigned int samples, const std::size_t width, const std::size_t height) :
  mNormal(glm::normalize(glm::vec3(0.f, 0.f, 1.f))),
  mMesh(
    // clang-format off
    std::vector<Vertex> ({
      // mirrored .texCoords.x
      { .position = { -2.5f, -2.5f * (float)height / (float)width, 0.f }, .normal = mNormal, .texCoords = { 1.f, 0.f } },
      { .position = {  2.5f, -2.5f * (float)height / (float)width, 0.f }, .normal = mNormal, .texCoords = { 0.f, 0.f } },
      { .position = {  2.5f,  2.5f * (float)height / (float)width, 0.f }, .normal = mNormal, .texCoords = { 0.f, 1.f } },
      { .position = { -2.5f,  2.5f * (float)height / (float)width, 0.f }, .normal = mNormal, .texCoords = { 1.f, 1.f } },
    }),
    std::vector<unsigned int> ({
      0, 1, 2,
      0, 2, 3
    })
    // clang-format on
  ),
  mRenderBuffer(samples, RenderBuffer::Format::RGB, width, height, 1)
{
}

void Mirror::screenshot(const Scene& scene, const glm::vec3& positionOffset) const
{
  const glm::vec3 position = this->position() + positionOffset;
  const glm::vec3 normal = glm::mat3(glm::transpose(glm::inverse(model()))) * mNormal;

  const glm::vec3 viewPos = scene.camera().position();
  const glm::vec3 incident = glm::normalize(position - viewPos);
  const glm::vec3 reflection = glm::reflect(incident, normal);

  Camera camera;
  camera.setPosition(position);
  camera.setDirection(reflection);

  glBindFramebuffer(GL_FRAMEBUFFER, mRenderBuffer.fbo());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  scene.render(camera);
  mRenderBuffer.blit();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Mirror::screenshot(const Scene& scene) const { screenshot(scene, glm::vec3(0.f)); }

void Mirror::render(const Shader& shader) const
{
  u_model::setUModel(shader, model());

  Lighting::u_material::bindTexture(shader, mRenderBuffer.textures(0));
  mMesh.draw();
  Lighting::u_material::unbindTexture(shader, mRenderBuffer.textures(0));
}

void Mirror::free() const
{
  mRenderBuffer.free();
  mMesh.free();
}
