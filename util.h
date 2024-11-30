#ifndef UTIL_H
#define UTIL_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

const int INITIAL_WINDOW_WIDTH = 800;
const int INITIAL_WINDOW_HEIGHT = 600;

GLFWwindow* createWindow();
bool initGlad();
void setupViewport(GLFWwindow* window);
void handleInput(GLFWwindow* window);
void update();
void render();

#endif
