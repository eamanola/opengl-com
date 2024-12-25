#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include "../scene.h"

#define POINTLIGHT_DEBUG

#include "../camera.h"
#include "../characters/tifa.h"
#include "../characters/dae.h"
#include "../characters/whipper.h"
#include "lighting.h"
#ifdef POINTLIGHT_DEBUG
#include "light.h"
#endif
#include "box.h"
#include "grass.h"
#include "window.h"
#include "mirror.h"
#include "skybox/skybox.h"
#include "skybox/skybox-reflector.h"
#include "../disco/floor.h"

class Playground : public Scene
{
public:
  Playground();
  ~Playground();

  void setup() override;
  void update(const float &time) override;
  void render() override;
  void teardown() override;

  void handleInput(const GLFWwindow* window) override;
  void onChar(const char c) override;
  void onMouse(const GLFWwindow* window, const double x, const double y) override;
  void onScroll(const GLFWwindow* window, const double x, const double y) override;

private:
  void toggleSpotLight() { mSpotlightOn = !mSpotlightOn; };
  void highlight(Box &box, glm::mat4 model);

  Shader mpSkeletal;
  Tifa tifa;
  Dae dae;
  Whipper whipper;

  Shader mpLighting;
  Model simpleModel;
  Box box;
  Grass grass;
  Window window;
  Mirror mirror;
  Floor floor;

  Shader mpSkybox;
  Skybox skybox;

  Shader mpReflectSkybox;
  SkyboxReflector skyboxReflector;

  #ifdef POINTLIGHT_DEBUG
  Light pointLightDebug;
  // Shader mpPlain;
  #endif

  Lighting lightingSettings;
  bool mSpotlightOn;

  float mLastFrame;
  float mLastX;
  float mLastY;
  bool mFirstMouse;
};

#endif
