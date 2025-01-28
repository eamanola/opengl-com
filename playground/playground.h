#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#define POINTLIGHT_DEBUG
#define NORMALS_DEBUG
#define DEFERRED

#include "box.h"
#include "camera.h"
#include "characters/dae.h"
#include "characters/icarus.h"
#include "characters/tifa.h"
#include "characters/whipper.h"
#include "disco/floor.h"
#include "draw-texture.h"
#include "grass.h"
#include "light-settings.h"
#include "mirror.h"
#include "render-buffer.h"
#include "scene.h"
#include "shaders/ub-proj-x-view.h"
#include "shadow-maps/shadow-maps.h"
#include "skybox/skybox-reflector.h"
#include "skybox/skybox.h"
#include "toy.h"
#include "wall.h"
#include "window.h"
#ifdef POINTLIGHT_DEBUG
#include "light-debug.h"
#endif
#ifdef DEFERRED
#include "shaders/deferred/g-buffer.h"
#endif

class Playground : public Scene
{
public:
  Playground();
  ~Playground();

  void setup() override;
  void update(const float& time) override;
  void render(unsigned int fbo, const Camera& camera) const override;
  void render() const override;
  using Scene::render;

  void renderShadowMap(const unsigned int fbo, const glm::mat4& projection, const glm::mat4& view)
    const override;

  void renderCubeMap(
    const unsigned int fbo,
    const std::vector<glm::mat4>& shadowMatrices,
    const glm::vec3& lightPos,
    const float& far
  ) const override;

  void teardown() override;

  void handleInput(const GLFWwindow* window) override;
  void onChar(const char c) override;
  void onMouse(const GLFWwindow* window, const double x, const double y) override;
  void onScroll(const GLFWwindow* window, const double x, const double y) override;

private:
  float mExposure;
  void addExposure(float exposure)
  {
    mExposure += exposure;

    if (mExposure > 3.5f)
      mExposure = 0.1f;

    std::cout << "Exposure: " << mExposure << "\n";
  }
  bool mSpotlightOn;
  void toggleSpotLight() { mSpotlightOn = !mSpotlightOn; }
  bool mBloomOn;
  void toggleBloom()
  {
    mBloomOn = !mBloomOn;

    std::cout << "Bloom: " << (mBloomOn ? "on" : "off") << "\n";
  }
  void highlight(Box& box, glm::mat4 model);
  void renderScene(
    const glm::mat4& projection,
    const glm::mat4& view,
    const glm::vec3& view_pos,
    const glm::vec3& view_dir,
    const Shader** shaders
  ) const;

  void renderDeffered(
    const glm::mat4& projection,
    const glm::mat4& view,
    const glm::vec3& view_pos,
    const glm::vec3& view_dir,
    const Shader** shaders
  ) const;

  void renderForward(
    const glm::mat4& projection,
    const glm::mat4& view,
    const glm::vec3& view_pos,
    const glm::vec3& view_dir,
    const Shader** shaders
  ) const;

  enum ShadersIndex {
    ISKELETAL,
    ISKELETAL_NORMAL_MAP,
    INORMAL_HEIGHT, // todo: line bug
    IINSTANCED,
#ifdef POINTLIGHT_DEBUG
    IPLAIN,
#endif
#ifdef NORMALS_DEBUG
    INORMALS,
#endif
    IREFLECT_SKYBOX,
    ILIGHTING_DEFF,
    ILIGHTING,
    IFLOOR,
    ISKYBOX,
  };

  Tifa tifa;
  Dae dae;
  Whipper whipper;

  Icarus icarus;

  Model simpleModel;
  Grass grass;
  Window window;
  Mirror mirror;

  Wall wall;
  Toy toy;

  Floor floor;

  Box box;

  Skybox skybox;

  SkyboxReflector skyboxReflector;

#ifdef POINTLIGHT_DEBUG
  LightDebug pointLightDebug;
#endif

  float mLastFrame;
  float mLastX;
  float mLastY;
  bool mFirstMouse;

  // renderers
#ifdef DEFERRED
  GBuffer gBuffer;
  Shader mpDeferredLighting;

  Shader mpGSkeletal;
  Shader mpGSkeletalNormal;
  Shader mpGNormalHeight;
  Shader mpGInstanced;
  Shader mpGLighting;
#else
  Shader mpSkeletal;
  Shader mpSkeletalNormal;
  Shader mpNormalHeight;
  Shader mpInstanced;
#endif
#ifdef POINTLIGHT_DEBUG
  Shader mpPlain;
#endif
#ifdef NORMALS_DEBUG
  Shader mpNormals;
#endif
  Shader mpReflectSkybox;
  Shader mpLighting;
  Shader mpFloor;
  Shader mpSkybox;

  // dir shadow renderers
  Shader mp_Skeletal_shadow;
  Shader mp_Lighting_shadow;
  Shader mp_Instanced_shadow;
#ifdef POINTLIGHT_DEBUG
  Shader mp_Plain_shadow;
#endif
  Shader mp_ReflectSkybox_shadow;
  Shader mp_Floor_shadow;

  // cube shadow renderers
  Shader mp_Skeletal_cshadow;
  Shader mp_Lighting_cshadow;
  Shader mp_Instanced_cshadow;
#ifdef POINTLIGHT_DEBUG
  Shader mp_Plain_cshadow;
#endif
  Shader mp_ReflectSkybox_cshadow;
  Shader mp_Floor_cshadow;

  ub_proj_x_view proj_x_view_ub;
  LightSettings lightingSettings;

#define BLOOM
// #define HDR
#ifdef BLOOM
#define HDR
#endif

#ifdef BLOOM
  Shader mpBrightness;
  Shader mpGaussian;
  RenderBuffer mGaussianBuffer;
#endif
#ifdef HDR
  Shader mpBloom;
  DrawTexture mDrawTexture;
  RenderBuffer mHDRBuffer;
#endif

  ShadowMaps mShadows;
};

#endif
