#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#define ANIMATION_TIME_NOT_STARTED -1

class Camera
{
public:
  Camera(/* args */);
  ~Camera();

  const glm::mat4 view() const;
  const glm::mat4 projection() const;
  const glm::vec3 position() const;
  const glm::vec3 front() const;
  const glm::vec3 right() const;
  void moveForward(const float speed = 0.05f);
  void moveBackward(const float speed = 0.05f);
  void moveLeft(const float speed = 0.05f);
  void moveRight(const float speed = 0.05f);
  void move(const glm::vec3 &direction, const float speed = 0.05f);
  void changeDirection(const float xoffset, const float yoffset);
  void zoom(const float yoffset);

  void setPosition(glm::vec3 &position, bool animate);
  void setPosition(glm::vec3 &position);
  void updatePosition(float time);

  void setDirection(glm::vec3 &direction, bool animate);
  void setDirection(glm::vec3 &direction);
  void updateDirection(float time);
private:
  glm::vec3 mPosition;
  glm::vec3 mFront;
  glm::vec3 mUp;
  glm::vec3 mRight;

  bool mAnimatePos;
  glm::vec3 mStartPos;
  glm::vec3 mTargetPos;
  float mDurationPos;
  float mStartTimePos;

  bool mAnimateDir;
  glm::vec3 mStartDir;
  glm::vec3 mTargetDir;
  float mDurationDir;
  float mStartTimeDir;

  float yaw;
  float pitch;
  float fov;

  void updateVectors(const glm::vec3 &direction);
};

#endif
