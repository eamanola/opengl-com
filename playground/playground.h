#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#define POINTLIGHT_DEBUG
// #define SHADOW_DEBUG

#include "camera.h"
#include "characters/dae.h"
#include "characters/icarus.h"
#include "characters/tifa.h"
#include "characters/whipper.h"
#include "light-settings.h"
#include "scene.h"
#ifdef POINTLIGHT_DEBUG
#include "light-debug.h"
#endif
#include "box.h"
#include "disco/floor.h"
#include "draw-texture.h"
#include "grass.h"
#include "mirror.h"
#include "render-buffer.h"
#include "shaders/ub-proj-x-view.h"
#include "shadow-maps/shadow-maps.h"
#include "skybox/skybox-reflector.h"
#include "skybox/skybox.h"
#include "toy.h"
#include "wall.h"
#include "window.h"

class Playground : public Scene
{
public:
  Playground();
  ~Playground();

  void setup() override;
  void update(const float& time) override;
  void render(const Camera& camera) const override;
  void render() const override;
  using Scene::render;

  void renderShadowMap(const glm::mat4& projection, const glm::mat4& view) const override;
  void renderCubeMap(
    const std::vector<glm::mat4>& shadowMatrices, const glm::vec3& lightPos, const float& far
  ) const override;
  void teardown() override;

  void handleInput(const GLFWwindow* window) override;
  void onChar(const char c) override;
  void onMouse(const GLFWwindow* window, const double x, const double y) override;
  void onScroll(const GLFWwindow* window, const double x, const double y) override;

private:
  void toggleSpotLight() { mSpotlightOn = !mSpotlightOn; };
  void highlight(Box& box, glm::mat4 model);
  void renderScene(
    const glm::mat4& projection,
    const glm::mat4& view,
    const glm::vec3& view_pos,
    const glm::vec3& view_dir,
    const Shader** shaders
  ) const;

  Shader mpSkeletal;
  Shader mp_Skeletal_shadow;
  Shader mp_Skeletal_cshadow;
  Tifa tifa;
  Dae dae;
  Whipper whipper;

  Shader mpSkeletalNormalMap;
  Icarus icarus;

  Shader mpLighting;
  Shader mp_Lighting_shadow;
  Shader mp_Lighting_cshadow;
  Model simpleModel;
  Grass grass;
  Window window;
  Mirror mirror;

  Shader mpLightingNormalHeightMap;
  Wall wall;
  Toy toy;

  Shader mpFloor;
  Shader mp_Floor_shadow;
  Shader mp_Floor_cshadow;
  Floor floor;

  Shader mpInstanced;
  Shader mp_Instanced_shadow;
  Shader mp_Instanced_cshadow;
  Box box;

  Shader mpSkybox;
  Skybox skybox;

  Shader mpReflectSkybox;
  Shader mp_ReflectSkybox_shadow;
  Shader mp_ReflectSkybox_cshadow;
  SkyboxReflector skyboxReflector;

#ifdef SHADOW_DEBUG
  Shader mpShadowsDebug;
#endif

#ifdef POINTLIGHT_DEBUG
  Shader mpPlain;
  Shader mp_Plain_shadow;
  Shader mp_Plain_cshadow;
  LightDebug pointLightDebug;
#endif

#define NORMALS_DEBUG
#ifdef NORMALS_DEBUG
  Shader mpNormals;
#endif

  LightSettings lightingSettings;

  bool mSpotlightOn;

  float mLastFrame;
  float mLastX;
  float mLastY;
  bool mFirstMouse;

  ub_proj_x_view proj_x_view_ub;

#define POST_PROCESS
#ifdef POST_PROCESS
  Shader mPostProcess;
  DrawTexture mDrawTexture;
  RenderBuffer mRBuffer;
#endif

  ShadowMaps mShadows;
};

#endif
