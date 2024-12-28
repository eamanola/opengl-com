#include "whipper.h"
#include <glm/glm.hpp>
#include "../shader-utils/shader-utils.h"
#include <glm/gtx/matrix_decompose.hpp>

// #define MERGE
Whipper::Whipper() : Character("assets/whipper/scene.gltf")
{
  mPosition = glm::vec3(0.f);
  mRotation = 0.f;

  setState(WHIPPER_STATES::DANCE);
}

#ifdef MERGE
const glm::vec3 Whipper::position() const { return glm::vec3(model()[3]); };
#else
const glm::vec3 Whipper::position() const { return mPosition; };
#endif

void Whipper::update(const float& time)
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

void Whipper::draw(const Shader &shader)
{
  const glm::mat4 model = this->model();

  glm::vec3 scale;
  glm::quat orientation;
  glm::vec3 translation;
  glm::vec3 foo;
  glm::vec4 bar;
  glm::decompose(model, scale, orientation, translation, foo, bar);

  glm::mat4 transform = glm::mat4(1.f);
  transform = glm::translate(transform, mPosition);
  transform = glm::translate(transform, translation);
  // transform = glm::translate(transform, mPosition + translation);
  transform = glm::rotate(transform, glm::radians(mRotation), glm::vec3(0.f, 1.f, 0.f));
  transform *= glm::toMat4(orientation);
  // transform *= glm::toMat4(glm::quat(glm::vec3(0.f, glm::radians(mRotation), 0.f)) * orientation);
  transform = glm::scale(transform, scale);


  setModel(transform);

  shader.setVec4fv("u_material.diffuse_color", glm::vec4(255.f/255.f, 192.f/255.f, 203.f/255.f, 1.0));
  shader.setVec4fv("u_material.specular_color", glm::vec4(0.2f, 0.2f, 0.2f, 1.0));

  Character::draw(shader);

  shader.setVec4fv("u_material.diffuse_color", glm::vec4(0.f));
  shader.setVec4fv("u_material.specular_color", glm::vec4(0.f));

  #ifdef MERGE
  mPosition = glm::vec3(0.f);
  mRotation = 0;
  #else
  setModel(model);
  #endif
}

void Whipper::handleInput(const GLFWwindow* window, const Scene &scene)
{
  const float time = glfwGetTime();
  const float deltaTime = time - lastFrame;
  lastFrame = time;

  bool SPACE = glfwGetKey((GLFWwindow*)window, GLFW_KEY_SPACE) == GLFW_PRESS;
  bool W = glfwGetKey((GLFWwindow*)window, GLFW_KEY_W) == GLFW_PRESS;
  bool S = glfwGetKey((GLFWwindow*)window, GLFW_KEY_S) == GLFW_PRESS;
  bool A = glfwGetKey((GLFWwindow*)window, GLFW_KEY_A) == GLFW_PRESS;
  bool D = glfwGetKey((GLFWwindow*)window, GLFW_KEY_D) == GLFW_PRESS;

  float angle = getAngle(W, D, S, A);

  if (angle >= 0 && mState != WHIPPER_STATES::LANDING)
  {
    glm::vec3 front = scene.cameraDir();
    float x = front.x * cos(glm::radians(angle)) - front.z * sin(glm::radians(angle));
    float z = front.x * sin(glm::radians(angle)) + front.z * cos(glm::radians(angle));
    glm::vec3 direction = glm::vec3(x, front.y, -z);

    #ifdef MERGE
    glm::vec3 scale;
    glm::quat orientation;
    glm::vec3 translation;
    glm::vec3 foo;
    glm::vec4 bar;
    glm::decompose(model(), scale, orientation, translation, foo, bar);
    glm::vec3 euler = glm::eulerAngles(orientation);
    const float prev = glm::degrees(euler.y);
    // float prev = std::floor(glm::degrees(asin(orientation.y) * 2));
    mRotation = angle - prev;

    std::cout << angle << " - " << prev << " = --" << mRotation << "--\n";
    #else
    mRotation = angle;
    #endif
    mPosition += direction * (2.5f * deltaTime);
  }

  WHIPPER_STATES newState;
  if(isJumping(mState)) newState = mState;
  else if(SPACE) {
    #ifdef MERGE
    jumpStartY = position().y;
    #endif
    newState = WHIPPER_STATES::JUMP_UP;
  }
  else if (angle >= 0) newState = WHIPPER_STATES::RUNNING;
  else newState = WHIPPER_STATES::IDLE;

  setState(newState);

  Character::handleInput(window, scene);
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
    #ifdef MERGE
    mPosition.y += 0.03f;
    #else
    mPosition.y = sin(frac * M_PI_2) * 3.0f;
    #endif
    if(frac >= 0.95f)
    {
      setState(WHIPPER_STATES::JUMP_DOWN);
    }
  }
  else if(mState == WHIPPER_STATES::JUMP_DOWN)
  {
    #ifdef MERGE
    mPosition.y -= 0.03f;
    #else
    mPosition.y = 0.0f + cos(frac * M_PI_2) * 3.0f;
    #endif

    if(frac >= 0.95f)
    {
      setState(WHIPPER_STATES::LANDING);
    }
  }
  else if(mState == WHIPPER_STATES::LANDING)
  {
    #ifdef MERGE
    glm::vec3 p = position();
    mPosition.y = jumpStartY - p.y;
    #else
    mPosition.y = 0.f;
    #endif
    if(frac >= 0.99f)
    {
      setState(WHIPPER_STATES::IDLE);
    }
  }
}

bool Whipper::isJumping(const WHIPPER_STATES &state)
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

