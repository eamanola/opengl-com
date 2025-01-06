#ifndef WHIPPER_H
#define WHIPPER_H

#include "character.h"

enum WHIPPER_STATES {
  RUNNING = 0,
  LANDING = 8,
  JUMP_UP = 9,
  DANCE = 11,
  IDLE = 14,
  JUMP_DOWN = 16
};

class Whipper : public Character
{
public:
  Whipper();
  ~Whipper() { }

  void handleInput(const GLFWwindow* window, const Scene& scene) override;
  void update(const float& time) override;
  void render(const Shader& shader) const override;

  const glm::vec3 position() const;

private:
  WHIPPER_STATES mState;

  float mRotation;
  float mJumpStartY;

  bool setState(WHIPPER_STATES state);
  bool isJumping(const WHIPPER_STATES& state);
  void updateJumping(float frac);

  float getAngle(bool W, bool D, bool S, bool A);
  float lastFrame = 0.f;
  void move(const glm::vec3& translation);
  void rotate(const float& rotation);
};

#endif
