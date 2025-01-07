#include "utils.h"
#include <glad/gl.h>

void Utils::screenshot(
  Scene& scene, const unsigned& FBO, const glm::vec3& position, const glm::vec3& normal
)
{
  Camera& camera = scene.camera();
  const glm::vec3 cameraPos = camera.position();
  const glm::vec3 cameraDir = camera.front();

  const glm::vec3 incident = glm::normalize(position - cameraPos);
  const glm::vec3 reflection = glm::reflect(incident, normal);

  camera.setPosition(position);
  camera.setDirection(reflection);

  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  scene.render();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  camera.setPosition(cameraPos);
  camera.setDirection(cameraDir);
}
