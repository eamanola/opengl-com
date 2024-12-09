#include <iostream>

#include "util.h"

void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %d, %s", error, description);
}

int main( int argc, char* args[] )
{
  GLFWwindow* window = createWindow();
  if(window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if(!initGlad())
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glfwSetErrorCallback(error_callback);

  setupViewport(window);
  setupMouse(window);
  setupKeyboard(window);

  initGL();

  setup();
  float deltaTime = 0.f;
  float lastFrame = 0.f;

  while (!glfwWindowShouldClose(window))
  {
    const float time = (float)glfwGetTime();
    deltaTime = time - lastFrame;
    lastFrame = time;

    handleInput(window, deltaTime);

    update();

    render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  shutdown();

  glfwTerminate();
  return 0;
}
