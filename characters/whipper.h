#ifndef WHIPPER_H
#define WHIPPER_H

#include "character.h"

enum WHIPPER_STATES
{
  RUNNING   = 0,
  LANDING   = 8,
  JUMP_UP   = 9,
  DANCE     = 11,
  IDLE      = 14,
  JUMP_DOWN = 16
};

class Whipper : public Character
{
public:
  Whipper();
  ~Whipper();

  void handleInput(const GLFWwindow* window, const Camera &camera) override;
  void update(float time) override;
  void draw(Shader &shader, const glm::mat4 &transform) override;
  void free() override;

  const glm::vec3 position() const;
  const float rotation() const;
private:
  unsigned int textureId;
  WHIPPER_STATES mState;

  glm::vec3 mPosition;
  float mRotation;

  bool setState(WHIPPER_STATES state);
  bool isJumping(const WHIPPER_STATES &state);
  void updateJumping(float frac);

  float getAngle(bool W, bool D, bool S, bool A);
};

#endif
