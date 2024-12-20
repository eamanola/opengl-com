#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "util.h"

Playground* playground;

void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %d, %s", error, description);
}

GLFWwindow* setup()
{
  GLFWwindow* window = createWindow();
  if(window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    return nullptr;
  }
  glfwMakeContextCurrent(window);

  if(!initGlad())
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return nullptr;
  }

  glfwSetErrorCallback(error_callback);
  setupViewport(window);
  setupMouse(window);
  setupKeyboard(window);
  initGL();

  return window;
}

void setPlayground(Playground* p)
{
  playground = p;
}

void handleInput(GLFWwindow* window)
{
  if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
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
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK); //default
  glFrontFace(GL_CCW); //default
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

void mouse_callback(GLFWwindow* window, double x, double y)
{
  playground->onMouse(window, x, y);
}

void scroll_callback(GLFWwindow* window, double x, double y)
{
  playground->onScroll(window, x, y);
}

void setupMouse(GLFWwindow* window)
{
  const bool DEBUG = false;
  glfwSetInputMode(window, GLFW_CURSOR, DEBUG ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
  playground->onChar((char)codepoint);
}

void setupKeyboard(GLFWwindow* window)
{
  glfwSetCharCallback(window, character_callback);
}
