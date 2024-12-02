#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "util.h"
#include "camera.h"
#include "cube.h"
#include "light.h"
#include <glm/glm.hpp>

Camera* camera;
Cube* cube;
Light* light;
glm::mat4 lightModel(1.0f);

void initGL()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1.f);
  glEnable(GL_DEPTH_TEST);

  light = new Light("shape.glvs", "light.glfs");
  cube = new Cube("shape.glvs", "shape.glfs");
  camera = new Camera();


  glm::vec3 lightPosition(1.2f, 1.0f, 2.0f);
  lightModel = glm::translate(lightModel, lightPosition);
  lightModel = glm::scale(lightModel, glm::vec3(0.2f));
  light->setModel(lightModel);

  cube->setModel(glm::mat4(1.0));

  glm::vec3 lightColor(1.f, 1.f, 1.f);
  light->program->use();
  light->program->setVec3("lightColor", glm::value_ptr(lightColor));

  cube->program->use();
  cube->program->setVec3("lightColor", glm::value_ptr(lightColor));
}

void update()
{
  cube->setView(camera->view());
  cube->setProjection(camera->projection());

  light->setView(camera->view());
  light->setProjection(camera->projection());
}

void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  cube->render();
  light->render();
}

void handleInput(GLFWwindow* window, float deltaTime)
{
  if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera->moveForward(2.5f * deltaTime);

  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera->moveBackward(2.5f * deltaTime);

  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera->moveLeft(2.5f * deltaTime);

  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera->moveRight(2.5f * deltaTime);
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

  camera->changeDirection(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double x, double y)
{
  camera->zoom((float)y);
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

  light->free();
  delete light;

  delete camera;
}
