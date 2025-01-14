#include "utils.h"
#include <glad/gl.h>

void Utils::screenshot(
  const Scene& scene, const unsigned& FBO, const glm::vec3& position, const glm::vec3& normal
)
{
  const glm::vec3 viewPos = scene.camera().position();

  const glm::vec3 incident = glm::normalize(position - viewPos);
  const glm::vec3 reflection = glm::reflect(incident, normal);

  Camera camera;
  camera.setPosition(position);
  camera.setDirection(reflection);

  glBindFramebuffer(GL_FRAMEBUFFER, FBO);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  scene.render(camera);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
