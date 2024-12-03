#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "util.h"
#include "camera.h"
#include "cube.h"
#include "light.h"
#include <glm/glm.hpp>
#include "rgba.h"


// #define DIRECTIONAL_LIGHT
#define SPOT_LIGHT
// #define POINT_LIGHT

#ifdef POINT_LIGHT
#define USE_ATTENUATION
#endif

#ifdef SPOT_LIGHT
#define USE_ATTENUATION
#endif

Camera camera;
Cube* cube;

#ifdef POINT_LIGHT
Light* light;
#endif

glm::vec3 lightPos(0.0f, 1.0f, 1.0f);


const glm::vec3 lightColor(1.f);
const float SHININESS = 32.f;
const glm::vec3 LIGHT_AMBIENT(0.2f);
const glm::vec3 LIGHT_DIFFUSE(0.5f);
const glm::vec3 LIGHT_SPECULAR(1.0f);

void initGL()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1.f);
  glEnable(GL_DEPTH_TEST);

  #if defined(POINT_LIGHT)
  light = new Light();
  light->program.use();
  light->program.setVec3("lightColor", (float *)glm::value_ptr(lightColor));

  const char* fshader = "./shaders/point-light.glfs";
  #elif defined(DIRECTIONAL_LIGHT)
  const char* fshader = "./shaders/directional-light.glfs";
  #elif defined(SPOT_LIGHT)
  const char* fshader = "./shaders/spot-light.glfs";
  #endif

  cube = new Cube("./shaders/phong.glvs", fshader);

  cube->program.use();
  cube->program.setFloat("material.shininess", SHININESS);
  cube->program.setVec3("light.ambient", (float *)glm::value_ptr(lightColor * LIGHT_AMBIENT));
  cube->program.setVec3("light.diffuse", (float *)glm::value_ptr(lightColor * LIGHT_DIFFUSE));
  cube->program.setVec3("light.specular", (float *)glm::value_ptr(LIGHT_SPECULAR));
  #ifdef USE_ATTENUATION
  cube->program.setFloat("light.attenuation.contant", 1.f);
  cube->program.setFloat("light.attenuation.linear", 0.09f);
  cube->program.setFloat("light.attenuation.quadratic", 0.032f);
  #endif
  #ifdef DIRECTIONAL_LIGHT
  cube->program.setVec3("light.direction", (float*)glm::value_ptr(glm::vec3(-0.2f, -1.0f, -0.3f)));
  #endif
  #ifdef SPOT_LIGHT
  cube->program.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
  cube->program.setFloat("light.outerCutOff", glm::cos(glm::radians(17.f)));
  #endif
}

void update()
{
  const glm::mat4 view = camera.view();
  const glm::mat4 projection = camera.projection();

  #ifdef POINT_LIGHT
  const float time = glfwGetTime();
  lightPos.x = 1.0f + sin(time) * 2.0f;
  lightPos.y = sin(time / 2.0f) * 1.0f;
  glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightPos);
  lightModel = glm::scale(lightModel, glm::vec3(0.2f));

  light->program.use();
  light->setView(view);
  light->setProjection(projection);
  light->setModel(lightModel);
  #endif

  cube->program.use();
  cube->setView(view);
  cube->setProjection(projection);
  cube->program.setVec3("viewPos", (float *)glm::value_ptr(camera.position()));
  #ifdef POINT_LIGHT
  cube->program.setVec3("light.position", (float *)glm::value_ptr(lightPos));
  #endif

  #ifdef SPOT_LIGHT
  cube->program.setVec3("light.position", (float *)glm::value_ptr(camera.position()));
  cube->program.setVec3("light.direction", (float*)glm::value_ptr(camera.front()));
  #endif
}

glm::vec3 cubePositions[] = {
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

void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  cube->program.use();
  for(unsigned int i = 0; i < 10; i++)
  {
    glm::mat4 boxModel = glm::translate(glm::mat4(1.0), cubePositions[i]);
    boxModel = glm::rotate(boxModel, glm::radians(20.f  * i), glm::vec3(1.0f, 0.3f, 0.5f));
    cube->setModel(boxModel);
    cube->render();
  }

  #ifdef POINT_LIGHT
  light->render();
  #endif
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

void shutdown()
{
  cube->free();
  delete cube;

  #ifdef POINT_LIGHT
  light->free();
  delete light;
  #endif
}
