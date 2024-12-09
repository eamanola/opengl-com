#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
  Camera(/* args */);
  ~Camera();

  const glm::mat4 view() const;
  const glm::mat4 projection() const;
  const glm::vec3 position() const;
  const glm::vec3 front() const;
  void moveForward(const float speed = 0.05f);
  void moveBackward(const float speed = 0.05f);
  void moveLeft(const float speed = 0.05f);
  void moveRight(const float speed = 0.05f);
  void changeDirection(const float xoffset, const float yoffset);
  void zoom(const float yoffset);

private:
  glm::vec3 mPosition;
  glm::vec3 mFront;
  glm::vec3 up;
  glm::vec3 right;

  float yaw;
  float pitch;
  float fov;

  void updateVectors(const glm::vec3 &direction);
};

#endif
