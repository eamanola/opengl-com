#ifndef UTIL_H
#define UTIL_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "scene.h"

const int INITIAL_WINDOW_WIDTH = 800;
const int INITIAL_WINDOW_HEIGHT = 600;

GLFWwindow* createWindow();
bool initGlad();
void initGL();
void setupViewport(GLFWwindow* window);
void setupMouse(GLFWwindow* window);
void setupKeyboard(GLFWwindow* window);
void handleInput(GLFWwindow* window);
GLFWwindow* setup();

void setScene(Scene* scene);

#endif
