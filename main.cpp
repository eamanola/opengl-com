// #define SHOW_DISCO

#include "util.h"
#include "playground/playground.h"
#ifdef SHOW_DISCO
#include "disco/disco.h"
#endif

int main( int argc, char* args[] )
{
  GLFWwindow* window = setup();
  if(window == nullptr)
  {
    glfwTerminate();
    return -1;
  }

#ifndef SHOW_DISCO
  Playground playground;
  setScene(&playground);
  playground.setup();
#else
  Disco disco;
  setScene(&disco);
  disco.setup();
#endif

  float lastFrame = 0;
  while (!glfwWindowShouldClose(window))
  {
    const float time = glfwGetTime();
    float fps = 1 / (time - lastFrame);
    lastFrame = time;
    std::cout << "FPS: " << fps << "\r";

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    handleInput(window);
#ifndef SHOW_DISCO
    playground.handleInput(window);
#else
    disco.handleInput(window);
#endif

#ifndef SHOW_DISCO
    playground.update(time);
#else
    disco.update(time);
#endif

#ifndef SHOW_DISCO
    playground.render();
#else
    disco.render();
#endif

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

#ifndef SHOW_DISCO
  playground.teardown();
#else
  disco.teardown();
#endif
  glfwTerminate();
  return 0;
}
