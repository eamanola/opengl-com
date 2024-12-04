#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "util.h"
#include "camera.h"
#include "cube.h"
#include "light.h"
#include <glm/glm.hpp>
#include "rgba.h"
#include <sstream>

Camera camera;
Cube* cube;
Light* light;

#define DIRECTIONAL_LIGHT
#define SPOT_LIGHT
#define POINT_LIGHT

#ifdef DIRECTIONAL_LIGHT
const glm::vec3 lightColor(1.f);
#endif

#ifdef SPOT_LIGHT
const glm::vec3 spotLightColor(1.f);
bool spotLightOff = true;
#endif

#ifdef POINT_LIGHT
const unsigned int NUM_POINT_LIGHTS = 4;
glm::vec3 lightPositions[] = {
	glm::vec3( 0.7f,  0.2f,  2.0f),
	glm::vec3( 2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3( 0.0f,  0.0f, -2.0f)
};
glm::vec3 lightColors[] = {
  glm::vec3(1.0f, 1.0f, 1.0f),
  glm::vec3(1.0f, 0.0f, 0.0f),
  glm::vec3(0.0f, 1.0f, 0.0f),
  glm::vec3(0.0f, 0.0f, 1.0f),
};
#endif

const unsigned int NUM_CUBES = 10;
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

const glm::vec3 LIGHT_AMBIENT(0.2f);
const glm::vec3 LIGHT_DIFFUSE(0.5f);
const glm::vec3 LIGHT_SPECULAR(1.0f);

void initGL()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.f);
  glEnable(GL_DEPTH_TEST);

  #ifdef POINT_LIGHT
  light = new Light();
  #endif

  cube = new Cube();

  cube->use();

  #ifdef DIRECTIONAL_LIGHT
  cube->setVec3("dirLight.direction", (float*)glm::value_ptr(glm::vec3(-0.2f, -1.0f, -0.3f)));
  // cube->setBool("dirLight.off", true);
  cube->setVec3("dirLight.light.ambient", (float *)glm::value_ptr(lightColor * LIGHT_AMBIENT));
  cube->setVec3("dirLight.light.diffuse", (float *)glm::value_ptr(lightColor * LIGHT_DIFFUSE));
  cube->setVec3("dirLight.light.specular", (float *)glm::value_ptr(LIGHT_SPECULAR));
  #endif

  #ifdef POINT_LIGHT
  for(unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
  {
    std::stringstream key;
    key << "pointLights[" << i << "]";
    cube->setVec3(key.str() + ".position", (float *)glm::value_ptr(lightPositions[i]));

    cube->setFloat(key.str() + ".attenuation.contant", 1.f);
    cube->setFloat(key.str() + ".attenuation.linear", 0.09f);
    cube->setFloat(key.str() + ".attenuation.quadratic", 0.032f);

    // if(i > 0)
      // cube->setBool(key.str() + ".light.off", true);
    cube->setVec3(key.str() + ".light.ambient", (float *)glm::value_ptr(lightColors[i] * LIGHT_AMBIENT));
    cube->setVec3(key.str() + ".light.diffuse", (float *)glm::value_ptr(lightColors[i] * LIGHT_DIFFUSE));
    cube->setVec3(key.str() + ".light.specular", (float *)glm::value_ptr(lightColors[i] * LIGHT_SPECULAR));
  }
  #endif

  #ifdef SPOT_LIGHT
  cube->setVec3("spotLight.direction", (float*)glm::value_ptr(camera.front()));
  cube->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
  cube->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.f)));
  cube->setVec3("spotLight.position", (float *)glm::value_ptr(camera.position()));

  cube->setFloat("spotLight.attenuation.contant", 1.f);
  cube->setFloat("spotLight.attenuation.linear", 0.09f);
  cube->setFloat("spotLight.attenuation.quadratic", 0.032f);

  cube->setVec3("spotLight.light.ambient", (float *)glm::value_ptr(spotLightColor * LIGHT_AMBIENT));
  cube->setVec3("spotLight.light.diffuse", (float *)glm::value_ptr(spotLightColor * LIGHT_DIFFUSE));
  cube->setVec3("spotLight.light.specular", (float *)glm::value_ptr(LIGHT_SPECULAR));
  cube->setBool("spotLight.light.off", spotLightOff);
  #endif

  cube->use(false);
}

void update()
{
  #ifdef POINT_LIGHT
  const float time = glfwGetTime();
  lightPositions[0].x = 1.0f + sin(time) * 2.0f;
  lightPositions[0].y = sin(time / 2.0f) * 1.0f;
  #endif
}

void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const glm::mat4 view = camera.view();
  const glm::mat4 projection = camera.projection();

  cube->use();
  cube->setView(view);
  cube->setProjection(projection);

  cube->setVec3("viewPos", (float *)glm::value_ptr(camera.position()));

  #ifdef POINT_LIGHT
  cube->setVec3("pointLights[0].position", (float *)glm::value_ptr(lightPositions[0]));
  #endif

  #ifdef SPOT_LIGHT
  cube->setVec3("spotLight.position", (float *)glm::value_ptr(camera.position()));
  cube->setVec3("spotLight.direction", (float*)glm::value_ptr(camera.front()));
  cube->setBool("spotLight.light.off", spotLightOff);
  #endif

  for(unsigned int i = 0; i < NUM_CUBES; i++)
  {
    glm::mat4 boxModel = glm::translate(glm::mat4(1.0), cubePositions[i]);
    boxModel = glm::rotate(boxModel, glm::radians(20.f  * i), glm::vec3(1.0f, 0.3f, 0.5f));

    cube->setModel(boxModel);
    cube->render();
  }

  cube->use(false);

  #ifdef POINT_LIGHT
  light->use();
  light->setView(view);
  light->setProjection(projection);
  for(unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
  {
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightPositions[i]);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    light->setVec3("color", (float *)glm::value_ptr(lightColors[i]));
    light->setModel(lightModel);
    light->render();
  }
  #endif
  light->use(false);
}

void switchSpotLight()
{
  spotLightOff = !spotLightOff;
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

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
  if((char)codepoint == 'f') {
    switchSpotLight();
  }
}

void setupKeyboard(GLFWwindow* window)
{
  glfwSetCharCallback(window, character_callback);
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
