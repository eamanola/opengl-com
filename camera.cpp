#include "camera.h"
#include <iostream>

const glm::vec3 UP = glm::vec3(0.f, 1.f, 0.f); // world UP

Camera::Camera(/* args */)
{
  position = glm::vec3(0.f, 0.f, 6.f);
  updateVectors(glm::vec3(0.f, 0.f, -1.f));

  pitch = 0.f;
  yaw = -90.f;
  fov = 45.f;
}

Camera::~Camera()
{
}

glm::mat4 Camera::view() const
{
  return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::projection() const
{
  return glm::perspective(glm::radians(fov), 800.f / 600.f, 0.1f, 100.f);
}

void Camera::moveForward(const float speed)
{
  position += front * speed;
}

void Camera::moveBackward(const float speed)
{
  position -= front * speed;
}

void Camera::moveLeft(const float speed)
{
  position -= right * speed;
}

void Camera::moveRight(const float speed)
{
  position += right * speed;
}

void Camera::updateVectors(const glm::vec3 &direction)
{
  front = glm::normalize(direction);
  right = glm::normalize(glm::cross(front, UP));
  up = glm::normalize(glm::cross(right, front));
}

void Camera::changeDirection(const float xoffset, const float yoffset)
{
  const float sensitivity = 0.1f;
  yaw += (xoffset * sensitivity);
  pitch += (yoffset * sensitivity);

  if(pitch > 89.f) {
    pitch = 89.f;
  }
  else if(pitch < -89.f) {
    pitch = -89.f;
  }

  const glm::vec3 direction = glm::vec3(
    cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
    sin(glm::radians(pitch)),
    sin(glm::radians(yaw)) * cos(glm::radians(pitch))
  );

  updateVectors(direction);
}

void Camera::zoom(const float yoffset)
{
  fov -= yoffset;
  if(fov < 1.f)
  {
    fov = 1.f;
  }
  else if(fov > 45.f)
  {
    fov = 45.f;
  }
}
