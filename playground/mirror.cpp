#include "mirror.h"
#include <glad/gl.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Mirror::Mirror(const float vWidth, const float vHeight)
:
mPosition(glm::vec3(2.5f, 0.5f, 0.f)),
mNormal(glm::normalize(glm::vec3(0.f, 0.f, 1.f))),
mMesh(
  std::vector<Vertex> ({
    // mirrored .texCoords.x
    { .position = { -0.5f, -(vHeight / vWidth / 2.f), 0.f }, .normal = mNormal, .texCoords = { 1.f, 0.f } },
    { .position = {  0.5f, -(vHeight / vWidth / 2.f), 0.f }, .normal = mNormal, .texCoords = { 0.f, 0.f } },
    { .position = {  0.5f,  (vHeight / vWidth / 2.f), 0.f }, .normal = mNormal, .texCoords = { 0.f, 1.f } },
    { .position = { -0.5f,  (vHeight / vWidth / 2.f), 0.f }, .normal = mNormal, .texCoords = { 1.f, 1.f } }
  }),
  std::vector<unsigned int> ({
    0, 1, 2,
    0, 2, 3
  })
),
mTexture(
  Texture {
    .type = TEXTURE_TYPE_DIFFUSE,
    .path = ""
  }
)
{
  setupFrameBuffer(vWidth, vHeight);

  glm::mat4 model = glm::mat4(1.0);
  model = glm::translate(model, mPosition);
  model = glm::rotate(model, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
  model = glm::scale(model, glm::vec3(2.f, 2.f, 2.f));
  setModel(model);
}

Mirror::~Mirror()
{
}

void Mirror::setupFrameBuffer(const float vWidth, const float vHeight)
{
  glGenFramebuffers(1, &mFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

  glGenTextures(1, &mTexture.id);
  glBindTexture(GL_TEXTURE_2D, mTexture.id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, vWidth, vHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture.id, 0);

  glGenRenderbuffers(1, &mRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, vWidth, vHeight);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cout << "frame buffer not complete\n";
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Mirror::screenshot(Scene &scene)
{
  Camera& camera = scene.camera();
  const glm::vec3 cameraPos = camera.position();
  const glm::vec3 cameraDir = camera.front();

  const glm::vec3 normal = glm::mat3(Drawable::model()) * mNormal;

  const glm::vec3 incident = glm::normalize(mPosition - cameraPos);
  const glm::vec3 reflection = glm::reflect(incident, normal);

  camera.setPosition(mPosition);
  camera.setDirection(reflection);

  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  scene.render();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  camera.setPosition(cameraPos);
  camera.setDirection(cameraDir);
}

void Mirror::draw(const Shader& shader)
{
  shader.setMat4fv("u_model", Drawable::model());

  mMesh.draw(shader, &mTexture);
}

void Mirror::free()
{
  glDeleteFramebuffers(1, &mFBO);
  glDeleteTextures(1, &mTexture.id);
  glDeleteRenderbuffers(1, &mRBO);
}
