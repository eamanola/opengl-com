#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "util.h"
#include "camera.h"
// #include "cube.h"
#include "box.h"
#include "light.h"
#include <glm/glm.hpp>
#include <sstream>
#include "model.h"
#include "lighting-shader.h"

Camera camera;
Box* box;
LightingShader* program;
Light* light;
Model* backpack;

const glm::vec3 LIGHT_AMBIENT(0.2f);
const glm::vec3 LIGHT_DIFFUSE(0.5f);
const glm::vec3 LIGHT_SPECULAR(1.0f);

#define DIRECTIONAL_LIGHT
#define SPOT_LIGHT
#define POINT_LIGHT

#ifdef DIRECTIONAL_LIGHT
const glm::vec3 lightColor(1.f);
LightingShader::DirLight dirLight {
  .direction = glm::vec3(-0.2f, -1.0f, -0.3f),
  .light = LightingShader::Light {
    // .off = true,
    .ambient = lightColor * LIGHT_AMBIENT,
    .diffuse = lightColor * LIGHT_DIFFUSE,
    .specular = LIGHT_SPECULAR
  }
};
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
LightingShader::PointLight pointLights[NUM_POINT_LIGHTS];
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

void initGL()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.f);
  glEnable(GL_DEPTH_TEST);


  #ifdef POINT_LIGHT
  light = new Light();
  #endif

  program = new LightingShader();

  program->use();
  backpack = new Model("./assets/backpack/backpack.obj");
  box = new Box();
  #ifdef DIRECTIONAL_LIGHT
  program->setDirLight(dirLight);
  #endif

  #ifdef POINT_LIGHT
  for(unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
  {
    pointLights[i] = {
      .position = lightPositions[i],
      .attenuation = {
        .constant = 1.f,
        .linear = 0.09f,
        .quadratic = 0.032f,
      },
      .light = {
        .ambient = lightColors[i] * LIGHT_AMBIENT,
        .diffuse = lightColors[i] * LIGHT_DIFFUSE,
        .specular = lightColors[i] * LIGHT_SPECULAR,
      }
    };
  }

  program->setPointLights(pointLights, NUM_POINT_LIGHTS);
  #endif

  #ifdef SPOT_LIGHT
  program->setSpotLight(
    LightingShader::SpotLight {
      .direction = camera.front(),
      .cutOff = glm::cos(glm::radians(12.5f)),
      .outerCutOff = glm::cos(glm::radians(17.5f)),
      .position = camera.position(),
      .attenuation = {
        .constant = 1.f,
        .linear = 0.09f,
        .quadratic = 0.032f
      },
      .light = {
        .off = spotLightOff,
        .ambient = spotLightColor * LIGHT_AMBIENT,
        .diffuse = spotLightColor * LIGHT_DIFFUSE,
        .specular = LIGHT_SPECULAR
      }
    }
  );
  #endif

  program->use(false);
}

void update()
{
  #ifdef POINT_LIGHT
  const float time = glfwGetTime();
  pointLights[0].position.x = 1.0f + sin(time) * 2.0f;
  pointLights[0].position.y = sin(time / 2.0f) * 1.0f;
  #endif
}

void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const glm::mat4 view = camera.view();
  const glm::mat4 projection = camera.projection();

  program->use();
  program->setView(view);
  program->setProjection(projection);

  program->setViewPosition(camera.position());

  #ifdef POINT_LIGHT
  program->setPointLight0Position(pointLights[0].position);
  #endif

  #ifdef SPOT_LIGHT
  program->updateSpotLight(
    camera.position(),
    camera.front(),
    spotLightOff
  );
  #endif

  for(unsigned int i = 0; i < NUM_CUBES; i++)
  {
    glm::mat4 boxModel = glm::translate(glm::mat4(1.0), cubePositions[i]);
    boxModel = glm::rotate(boxModel, glm::radians(20.f  * i), glm::vec3(1.0f, 0.3f, 0.5f));

    program->setModel(boxModel);
    box->draw(*program);
  }

  program->use(false);

  #ifdef POINT_LIGHT
  light->use();
  light->setView(view);
  light->setProjection(projection);
  for(unsigned int i = 0; i < NUM_POINT_LIGHTS; i++)
  {
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0), pointLights[i].position);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    light->setVec3("color", (float *)glm::value_ptr(lightColors[i]));
    light->setModel(lightModel);
    light->render();
  }
  light->use(false);
  #endif

  glm::mat4 bpm = glm::translate(glm::mat4(1.0), glm::vec3(0.f, 0.f, 2.f));
  bpm = glm::scale(bpm, glm::vec3(0.2f));

  program->use();
  program->setModel(bpm);
  backpack->draw(*program);
  program->use(false);
}

#ifdef SPOT_LIGHT
void switchSpotLight()
{
  spotLightOff = !spotLightOff;
}
#endif

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
  #ifdef SPOT_LIGHT
  if((char)codepoint == 'f') {
    switchSpotLight();
  }
  #endif
}

void setupKeyboard(GLFWwindow* window)
{
  glfwSetCharCallback(window, character_callback);
}

void shutdown()
{
  #ifdef POINT_LIGHT
  light->use();
  light->free();
  delete light;
  #endif

  program->use();

  backpack->free();
  delete backpack;

  box->free();
  delete box;

  program->free();
  delete program;
}
