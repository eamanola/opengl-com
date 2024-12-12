#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "util.h"
#include "camera.h"
#include "box.h"
#include "light.h"
#include "model.h"
#include "lighting.h"

#define SKELETAL

#ifdef SKELETAL
#include "skeletal/skeletal-model.h"
#endif


Camera camera;
Shader* program;
Lighting* lighting;
Shader* plainProgram;
Light* light;
Box* box;
Model* m2b;

#ifdef SKELETAL
Shader* skeletal;
SkeletalModel* tifa;
SkeletalModel* runner;
SkeletalModel* whipper;
Lighting* lighting2;
#endif

#define SPOT_LIGHT
#define POINT_LIGHT

#ifdef SPOT_LIGHT
bool spotLightOff = true;
#endif

unsigned int selectedBox = -1;
const unsigned int NUM_BOXES = 10;
glm::vec3 boxPositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

void setup()
{
  #ifdef POINT_LIGHT
  plainProgram = new Shader("./shaders/plain.glvs", "./shaders/single-color.glfs");
  light = new Light();
  #endif

  program = new Shader("./shaders/lighting.glvs", "./shaders/lighting.glfs");
  program->use();
  lighting = new Lighting(*program);
  program->use(false);
  m2b = new Model();
  m2b->loadModel("assets/2b-jumps2/scene.gltf");
  box = new Box();

#ifdef SKELETAL
  skeletal = new Shader("./skeletal/skeletal.glvs", "./shaders/lighting.glfs");
  skeletal->use();
  lighting2 = new Lighting(*skeletal);
  for(unsigned int i = 0; i < 100; i++)
  {
    skeletal->setMat4fv("u_bone_transforms[" + std::to_string(i) + "]", glm::mat4(0.f));
  }
  skeletal->use(false);
  tifa = new SkeletalModel();
  tifa->loadModel("assets/tifa/scene.gltf");

  runner = new SkeletalModel();
  runner->loadModel("assets/dae-runner/model.dae");

  whipper = new SkeletalModel();
  whipper->loadModel("assets/whipper/scene.gltf");
  whipper->setAnimation(11);
#endif
}

void highlight(Box &box, glm::mat4 model)
{
  glEnable(GL_STENCIL_TEST);
  glClear(GL_STENCIL_BUFFER_BIT);

  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilMask(0xFF);
  program->use();
  program->setMat4fv("u_model", model);
  box.draw(*program);
  program->use(false);

  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  glStencilMask(0x00);
  // glDisable(GL_DEPTH_TEST); ?
    plainProgram->use();
    plainProgram->setVec3fv("color", glm::vec3(1.0, 1.0, 1.0));
    plainProgram->setMat4fv("u_model", glm::scale(model, glm::vec3(1.1)));
    box.draw(*plainProgram);
    plainProgram->use(false);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
  // glEnable(GL_DEPTH_TEST);

  glDisable(GL_STENCIL_TEST);
}

void update()
{
  #ifdef POINT_LIGHT
  const float time = glfwGetTime();
  lighting->mLights.positions[0].x = 1.0f + sin(time) * 2.0f;
  lighting->mLights.positions[0].y = sin(time / 2.0f) * 1.0f;

  lighting2->mLights.positions[0].x = 1.0f + sin(time) * 2.0f;
  lighting2->mLights.positions[0].y = sin(time / 2.0f) * 1.0f;
  #endif

#ifdef SKELETAL
  tifa->update(time);
  runner->update(time);
  whipper->update(time);
#endif
}

void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const glm::mat4 view = camera.view();
  const glm::mat4 projection = camera.projection();
  const glm::mat4 u_proj_x_view = projection * view;
  program->use();
  // mult view proj?
  // program->setMat4fv("view", view);
  // program->setMat4fv("projection", projection);
  program->setMat4fv("u_proj_x_view", u_proj_x_view);

  lighting->setViewPosition(camera.position());
  #ifdef POINT_LIGHT
  lighting->updatePointLight0Position();
  #endif

  #ifdef SPOT_LIGHT
  lighting->updateSpotLight(camera.position(), camera.front(), spotLightOff);
  #endif

  for(unsigned int i = 0; i < NUM_BOXES; i++)
  {
    glm::mat4 boxModel = glm::translate(glm::mat4(1.0), boxPositions[i]);
    boxModel = glm::rotate(boxModel, glm::radians(20.f  * i), glm::vec3(1.0f, 0.3f, 0.5f));

    if(i == selectedBox)
    {
      highlight(*box, boxModel);
      program->use();
    } else {
      program->setMat4fv("u_model", boxModel);
      box->draw(*program);
    }
  }

  glm::mat4 m2bm = glm::translate(glm::mat4(1.0), glm::vec3(0.f, 1.f, 0.f));
  program->setMat4fv("u_model", m2bm);
  m2b->draw(*program);

  program->use(false);

  #ifdef POINT_LIGHT
  plainProgram->use();
  plainProgram->setMat4fv("u_proj_x_view", u_proj_x_view);
  for(unsigned int i = 0; i < lighting->NR_POINT_LIGHTS; i++)
  {
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lighting->mLights.positions[i]);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    plainProgram->setVec3fv("color", lighting->mLights.colors[i]);
    plainProgram->setMat4fv("u_model", lightModel);
    light->draw(*plainProgram);
  }
  plainProgram->use(false);
  #endif
#ifdef SKELETAL
  skeletal->use();

  lighting2->setViewPosition(camera.position());
  lighting2->updatePointLight0Position();

  skeletal->setMat4fv("u_proj_x_view", u_proj_x_view);

  glm::mat4 skeletalm;

  // dae
  const std::vector<glm::mat4> runnerTransforms = runner->pose();
  for(unsigned int i = 0; i < runnerTransforms.size(); i++)
  {
    skeletal->setMat4fv("u_bone_transforms[" + std::to_string(i) + "]", runnerTransforms[i]);
  }
  skeletalm = glm::mat4(1.0);
  skeletalm = glm::translate(skeletalm, glm::vec3(0.f, -1.f, 1.f));
  skeletalm = glm::rotate(skeletalm, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  skeletalm = glm::scale(skeletalm, glm::vec3(0.25f));
  skeletal->setMat4fv("u_model", skeletalm);
  runner->draw(*skeletal);

  // whipper
  const std::vector<glm::mat4> whipperTransforms = whipper->pose();
  for(unsigned int i = 0; i < whipperTransforms.size(); i++)
  {
    skeletal->setMat4fv("u_bone_transforms[" + std::to_string(i) + "]", whipperTransforms[i]);
  }
  skeletalm = glm::mat4(1.0);
  skeletalm = glm::translate(skeletalm, glm::vec3(1.5f, -1.f, 1.f));
  skeletalm = glm::rotate(skeletalm, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  skeletalm = glm::scale(skeletalm, glm::vec3(0.15f));
  skeletal->setMat4fv("u_model", skeletalm);
  // whipper->draw(*skeletal);

  // tifa
  const std::vector<glm::mat4> tifaTransforms = tifa->pose();
  for(unsigned int i = 0; i < tifaTransforms.size(); i++)
  {
    skeletal->setMat4fv("u_bone_transforms[" + std::to_string(i) + "]", tifaTransforms[i]);
  }
  skeletalm = glm::mat4(1.0);
  skeletalm = glm::translate(skeletalm, glm::vec3(-1.5f, -1.f, 1.f));
  skeletalm = glm::rotate(skeletalm, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
  skeletal->setMat4fv("u_model", skeletalm);
  tifa->draw(*skeletal);

  skeletal->use(false);
#endif
}

#ifdef SPOT_LIGHT
void switchSpotLight()
{
  spotLightOff = !spotLightOff;
}
#endif

void selectNext()
{
  selectedBox ++;
  if(selectedBox >= NUM_BOXES) selectedBox = 0;

  std::cout << selectedBox;
}

void handleInput(GLFWwindow* window, float deltaTime)
{
  if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.moveForward(2.5f * deltaTime);

  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.moveBackward(2.5f * deltaTime);

  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.moveLeft(2.5f * deltaTime);

  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.moveRight(2.5f * deltaTime);
}

GLFWwindow* createWindow()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(
    INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "OpenGL", NULL, NULL
  );
  return window;
}

bool initGlad()
{
  return gladLoadGL(glfwGetProcAddress);
}

void initGL()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.f);
  glEnable(GL_DEPTH_TEST);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void setupViewport(GLFWwindow* window)
{
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  framebuffer_size_callback(window, width, height);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

float lastX = 400; float lastY = 300;
bool firstMouse = true;;
void mouse_callback(GLFWwindow* window, double x, double y)
{
  if(firstMouse)
  {
    lastX = x;
    lastY = y;
    firstMouse = false;
  }

  const float xoffset = x - lastX;
  const float yoffset = y - lastY;
  lastX = x;
  lastY = y;

  camera.changeDirection(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double x, double y)
{
  camera.zoom((float)y);
}

void setupMouse(GLFWwindow* window)
{
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
  #ifdef SPOT_LIGHT
  if((char)codepoint == 'f') {
    switchSpotLight();
  }
  #endif
  if((char)codepoint == 'n') {
    selectNext();
  }

}

void setupKeyboard(GLFWwindow* window)
{
  glfwSetCharCallback(window, character_callback);
}

void shutdown()
{
  #ifdef POINT_LIGHT
  plainProgram->use();

  light->free();
  delete light;

  plainProgram->free();
  delete plainProgram;
  #endif

  program->use();

  m2b->free();
  delete m2b;

  box->free();
  delete box;

  delete lighting;

  program->free();
  delete program;

#ifdef SKELETAL
  skeletal->use();
  tifa->free();
  runner->free();
  whipper->free();
  delete tifa;
  delete runner;
  delete whipper;
  skeletal->free();
  delete lighting2;
  delete skeletal;
#endif
}
