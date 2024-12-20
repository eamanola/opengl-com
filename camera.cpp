#include "camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 UP = glm::vec3(0.f, 1.f, 0.f); // world UP

Camera::Camera(/* args */)
{
  mPosition = glm::vec3(0.f, 0.f, 6.f);
  updateVectors(glm::vec3(0.f, 0.f, -1.f));

  pitch = 0.f;
  yaw = -90.f;
  fov = 45.f;

  mAnimatePos = false;
  mDurationPos = 1.f;

  mAnimateDir = false;
  mDurationDir = 1.f;
}

Camera::~Camera()
{
}

const glm::mat4 Camera::view() const
{
  return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

const glm::mat4 Camera::projection() const
{
  return glm::perspective(glm::radians(fov), 800.f / 600.f, 0.1f, 100.f);
}

const glm::vec3 Camera::position() const
{
  return mPosition;
}

const glm::vec3 Camera::front() const
{
  return mFront;
}

const glm::vec3 Camera::right() const
{
  return mRight;
}

void Camera::moveForward(const float speed)
{
  move(mFront, speed);
}

void Camera::moveBackward(const float speed)
{
  const glm::vec3 back = -mFront;
  move(back, speed);
}

void Camera::moveLeft(const float speed)
{
  const glm::vec3 left = -mRight;
  move(left, speed);
}

void Camera::moveRight(const float speed)
{
  move(mRight, speed);
}

void Camera::move(const glm::vec3 &direction, const float speed)
{
  mPosition += direction * speed;
}

void Camera::updateVectors(const glm::vec3 &direction)
{
  mFront = glm::normalize(direction);
  mRight = glm::normalize(glm::cross(mFront, UP));
  mUp = glm::normalize(glm::cross(mRight, mFront));
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

void Camera::setPosition(const glm::vec3 position, const bool animate)
{
  mAnimatePos = animate;

  if(animate)
  {
    mStartPos = mPosition;
    mTargetPos = position;
    mStartTimePos = ANIMATION_TIME_NOT_STARTED;
  }
  else
  {
    mPosition = position;
  }
}

void Camera::updatePosition(const float time)
{
  if(mAnimatePos)
  {
    if(mStartTimePos == ANIMATION_TIME_NOT_STARTED)
    {
      mStartTimePos = time;
    }

    const float frac = std::min((time - mStartTimePos) / mDurationPos, 1.f);
    mPosition = glm::mix(mStartPos, mTargetPos, frac);
    mAnimatePos = mPosition != mTargetPos;
  }
}


void Camera::setDirection(const glm::vec3 direction, const bool animate)
{
  mAnimateDir = animate;

  if(animate)
  {
    mStartDir = mFront;
    mTargetDir = glm::normalize(direction);
    mStartTimeDir = ANIMATION_TIME_NOT_STARTED;
  }
  else
  {
    updateVectors(direction);
  }
}

void Camera::updateDirection(const float time)
{
  if(mAnimateDir)
  {
    if(mStartTimeDir == ANIMATION_TIME_NOT_STARTED)
    {
      mStartTimeDir = time;
    }

    const float frac = std::min((time - mStartTimeDir) / mDurationDir, 1.f);
    const glm::vec3 direction = glm::mix(mStartDir, mTargetDir, frac);
    updateVectors(direction);
    mAnimateDir = mFront != mTargetDir;
  }
}
