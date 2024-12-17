#include "whipper.h"
#include <glm/glm.hpp>
#include "../shader-utils.h"

Whipper::Whipper() : Character("assets/whipper/scene.gltf")
{
  glm::mat4 model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.f, -1.f, 0.f));
  model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  model = glm::scale(model, glm::vec3(0.15f));
  setModelMatrix(model);

  mPosition = glm::vec3(1.5f, 0.f, 1.f);
  mRotation = 0.f;

  setState(WHIPPER_STATES::DANCE);

  textureId = ShaderUtils::loadTexture("assets/pink.png");
}

Whipper::~Whipper()
{
}

void Whipper::update(float time)
{
  bool loop = !isJumping(mState);
  float frac;

  if (loop)
  {
    Character::update(time);
  }
  else {
    Character::update(time - mStartTime);
    frac = (time - mStartTime) * mCurrentAnimation->ticksPerSecond / mCurrentAnimation->duration;
  }

  if(isJumping(mState))
  {
    updateJumping(frac);
  }
}

void Whipper::draw(Shader &shader, const glm::mat4 &transform)
{
  shader.use();
  glActiveTexture(GL_TEXTURE0);
  shader.setInt("u_material.texture_diffuse1", 0);
  glBindTexture(GL_TEXTURE_2D, textureId);

  Character::draw(shader, transform);

  glActiveTexture(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}


const glm::vec3 Whipper::position() const
{
  return mPosition;
}

const float Whipper::rotation() const
{
  return mRotation;
}

float lastFrame1 = 0.f;
void Whipper::handleInput(GLFWwindow* window, Camera &camera)
{
  const float time = glfwGetTime();
  const float deltaTime = time - lastFrame1;
  lastFrame1 = time;

  bool SPACE = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
  bool W = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
  bool S = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
  bool A = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
  bool D = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

  float angle = getAngle(W, D, S, A);

  if (angle >= 0 && mState != WHIPPER_STATES::LANDING)
  {
    glm::vec3 front = camera.front();
    float x = front.x * cos(glm::radians(angle)) - front.z * sin(glm::radians(angle));
    float z = front.x * sin(glm::radians(angle)) + front.z * cos(glm::radians(angle));
    glm::vec3 direction = glm::vec3(x, front.y, -z);

    mRotation = angle;
    mPosition += direction * (2.5f * deltaTime);
  }

  WHIPPER_STATES newState;
  if(isJumping(mState)) newState = mState;
  else if(SPACE) newState = WHIPPER_STATES::JUMP_UP;
  else if (angle >= 0) newState = WHIPPER_STATES::RUNNING;
  else newState = WHIPPER_STATES::IDLE;

  setState(newState);
}

void Whipper::free()
{
  Character::free();

  glDeleteTextures(1, &textureId);
}


bool Whipper::setState(WHIPPER_STATES state)
{
  if(mState != state)
  {
    if(setAnimation((unsigned int)state))
    {
      mState = state;
      return true;
    }
  }

  return false;
}

void Whipper::updateJumping(float frac)
{
  if(mState == WHIPPER_STATES::JUMP_UP)
  {
    mPosition.y = sin(frac * M_PI_2) * 3.0f;
    if(frac >= 0.95f)
    {
      setState(WHIPPER_STATES::JUMP_DOWN);
    }
  }
  else if(mState == WHIPPER_STATES::JUMP_DOWN)
  {
    mPosition.y = 0.0f + cos(frac * M_PI_2) * 3.0f;
    if(frac >= 0.95f)
    {
      setState(WHIPPER_STATES::LANDING);
    }
  }
  else if(mState == WHIPPER_STATES::LANDING)
  {
    mPosition.y = 0.f;
    if(frac >= 0.99f)
    {
      setState(WHIPPER_STATES::IDLE);
    }
  }
}

bool Whipper::isJumping(WHIPPER_STATES state)
{
  return state == WHIPPER_STATES::JUMP_UP
    || state == WHIPPER_STATES::JUMP_DOWN
    || state == WHIPPER_STATES::LANDING;
}

float Whipper::getAngle(bool W, bool D, bool S, bool A)
{
  float angle = -1.f;

  if (S & D) angle = 45.f;
  else if (D & W) angle = 135.f;
  else if (W & A) angle = 225.f;
  else if (A & S) angle = 315.f;
  else if (S) angle = 0.f;
  else if(D) angle = 90.f;
  else if (W) angle = 180.f;
  else if (A) angle = 270.f;

  return angle;
}

