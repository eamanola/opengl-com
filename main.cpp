#include <iostream>

#include "util.h"
#include "triangle.h"

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
  Triangle triangle;
  triangle.compile();

  while (!glfwWindowShouldClose(window))
  {
    handleInput(window);

    update();

    render();

    triangle.render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  triangle.free();
  glfwTerminate();
  return 0;
}
