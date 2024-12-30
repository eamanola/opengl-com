#ifndef DISCO_H
#define DISCO_H

#include "../scene.h"

// #define DISCO_DEBUG

#include "../camera.h"
#include "../characters/tifa.h"
#include "../characters/dae.h"
#include "../characters/whipper.h"
#ifdef DISCO_DEBUG
#include "../shaders/plain-shader.h"
#include "../playground/light-debug.h"
#endif
#include "../playground/mirror.h"
#include "../playground/skybox/skybox.h"
#include "floor.h"
#include "lighting-settings.h"
#include "../shaders/lighted-shader.h"

class Disco : public Scene
{
public:
  Disco();
  ~Disco();

  void setup() override;
  void update(const float &time) override;
  void render() override;
  void teardown() override;

  #ifdef DISCO_DEBUG
  void drawLightDebugs();
  void handleInput(const GLFWwindow* window) override;
  void onChar(const char c) override {};
  void onMouse(const GLFWwindow* window, const double x, const double y) override;
  void onScroll(const GLFWwindow* window, const double x, const double y) override;
  #endif

private:
  LightingSettings lightingSettings;

  LightedShader mpSkeletal;
  Tifa tifa;
  Dae dae;
  Whipper whipper;

  LightedShader mpLighting;
  Mirror mirror;
  Floor floor;

  Shader mpSkybox;
  Skybox skybox;

  #ifdef DISCO_DEBUG
  PlainShader mpPlain;
  LightDebug pointLightDebug;
  #endif

  float mLastFrame;
  float mLastX;
  float mLastY;
  bool mFirstMouse;
};

#endif
