#ifndef DISCO_H
#define DISCO_H

#include "../scene.h"

#define DISCO_DEBUG

#include "../camera.h"
#include "../characters/tifa.h"
#include "../characters/dae.h"
#include "../characters/whipper.h"
#ifdef DISCO_DEBUG
#include "../playground/light.h"
#endif
#include "../playground/mirror.h"
#include "../playground/skybox/skybox.h"
#include "floor.h"
#include "lighting-settings.h"

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
  void handleInput(const GLFWwindow* window) override;
  void onChar(const char c) override {};
  void onMouse(const GLFWwindow* window, const double x, const double y) override;
  void onScroll(const GLFWwindow* window, const double x, const double y) override;
  #endif

private:
  Shader mpSkeletal;
  Tifa tifa;
  Dae dae;
  Whipper whipper;

  Shader mpLighting;
  Mirror mirror;
  Floor floor;

  Shader mpSkybox;
  Skybox skybox;

  #ifdef DISCO_DEBUG
  Light pointLightDebug;
  #endif

  LightingSettings lightingSettings;

  float mLastFrame;
  float mLastX;
  float mLastY;
  bool mFirstMouse;
};

#endif
