#include "util.h"

int main( int argc, char* args[] )
{
  GLFWwindow* window = setup();
  if(window == nullptr)
  {
    glfwTerminate();
    return -1;
  }

  Playground playground;
  playground.setup();
  setPlayground(&playground);

  float lastFrame = 0;
  while (!glfwWindowShouldClose(window))
  {
    const float time = glfwGetTime();
    float fps = 1 / (time - lastFrame);
    lastFrame = time;
    std::cout << "FPS: " << fps << "\r";

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    handleInput(window);
    playground.handleInput(window);

    playground.update(glfwGetTime());

    playground.render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  playground.teardown();

  glfwTerminate();
  return 0;
}
