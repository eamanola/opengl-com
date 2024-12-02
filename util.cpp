#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "util.h"
#include "camera.h"

Camera* camera;
Shape* shape;

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

void update()
{
  shape->setView(camera->view());
  shape->setProjection(camera->projection());
}

void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shape->render();
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

void initGL()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1.f);
  glEnable(GL_DEPTH_TEST);
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

void setCamera(Camera* aCamera)
{
  camera = aCamera;
}

void setShape(Shape* aShape)
{
  shape = aShape;
}
