#include "playground.h"
#include "shaders/locations.h"

// #define FOLLOW_WHIPPER

#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 4
#define NUM_SPOT_LIGHTS 1

#define DIR_LIGHT_DISTANCE 3.f
#define CUBE_FAR 15.f
#define SAMPLES 4

Playground::Playground() :
  simpleModel("assets/2b-jumps2/scene.gltf"),
  mirror(SAMPLES, 800, 600),
  floor(25, 25),

#ifdef DEFERRED
  gBuffer(800, 600),
  mpDeferredLighting(
    "./shaders/deferred/lighting.vs",
    nullptr,
    "./shaders/deferred/lighting.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
      "#define IN_RENDER_SHADOWS\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
  mpGSkeletal(
    "./shaders/deferred/geometry.vs",
    nullptr,
    "./shaders/deferred/geometry.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
      "#define SKELETAL\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
  mpGSkeletalNormal(
    "./shaders/deferred/geometry.vs",
    nullptr,
    "./shaders/deferred/geometry.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
      "#define SKELETAL\n",
      "#define IN_NORMAL_MAP\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
  mpGNormalHeight(
    "./shaders/deferred/geometry.vs",
    nullptr,
    "./shaders/deferred/geometry.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
      "#define IN_NORMAL_MAP\n",
      "#define IN_HEIGHT_MAP\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
  mpGInstanced(
    "./shaders/deferred/geometry.vs",
    nullptr,
    "./shaders/deferred/geometry.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
      "#define INSTANCED\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
  mpGLighting(
    "./shaders/deferred/geometry.vs",
    nullptr,
    "./shaders/deferred/geometry.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
#else
  mpSkeletal(
    "./shaders/lighting.vs",
    nullptr,
    "./shaders/lighting.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
      "#define SKELETAL\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
  mpSkeletalNormal(
    "./shaders/lighting.vs",
    nullptr,
    "./shaders/lighting.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
      "#define SKELETAL\n",
      "#define IN_NORMAL_MAP\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
  mpNormalHeight(
    "./shaders/lighting.vs",
    nullptr,
    "./shaders/lighting.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
      "#define IN_NORMAL_MAP\n",
      "#define IN_HEIGHT_MAP\n",
      "#define IN_RENDER_SHADOWS\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
  mpInstanced(
    "./shaders/lighting.vs",
    nullptr,
    "./shaders/lighting.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
      "#define INSTANCED\n",
      "#define IN_RENDER_SHADOWS\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
#endif
#ifdef POINTLIGHT_DEBUG
  mpPlain("./shaders/plain.vs", nullptr, "./shaders/single-color.fs"),
#endif
#ifdef NORMALS_DEBUG
  mpNormals(
#ifdef DEFERRED
    "./shaders/deferred/geometry.vs",
#else
    "./shaders/lighting.vs",
#endif
    "./shaders/draw-normals.gs",
    "./shaders/single-color.fs",
    {
      "#define INSTANCED\n",
      "#define NORMAL\n",
    }
  ),
#endif
  mpReflectSkybox(
    "./playground/skybox/reflect-skybox.vs", nullptr, "./playground/skybox/reflect-skybox.fs"
  ),
  mpLighting(
    "./shaders/lighting.vs",
    nullptr,
    "./shaders/lighting.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
  mpFloor(
    "./disco/floor.vs",
    nullptr,
    "./shaders/lighting.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
      "#define IN_V_COLOR\n",
      "#define IN_RENDER_SHADOWS\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
  mpSkybox("./playground/skybox/cube.vs", nullptr, "./playground/skybox/cube.fs"),

  mp_Skeletal_shadow(
    "./shaders/lighting.vs", nullptr, "./shadow-maps/simple-depth.fs", { "#define SKELETAL\n" }
  ),
  mp_Lighting_shadow("./shaders/lighting.vs", nullptr, "./shadow-maps/simple-depth.fs"),
  mp_Instanced_shadow(
    "./shaders/lighting.vs", nullptr, "./shadow-maps/simple-depth.fs", { "#define INSTANCED\n" }
  ),
#ifdef POINTLIGHT_DEBUG
  mp_Plain_shadow("./shaders/plain.vs", nullptr, "./shadow-maps/simple-depth.fs"),
#endif
  mp_ReflectSkybox_shadow(
    "./playground/skybox/reflect-skybox.vs", nullptr, "./shadow-maps/simple-depth.fs"
  ),
  mp_Floor_shadow("./disco/floor.vs", nullptr, "./shadow-maps/simple-depth.fs"),

  mp_Skeletal_cshadow(
    "./shaders/lighting.vs",
    "./shadow-maps/cube-depth.gs",
    "./shadow-maps/cube-depth.fs",
    { "#define SKELETAL\n" }
  ),
  mp_Lighting_cshadow(
    "./shaders/lighting.vs", "./shadow-maps/cube-depth.gs", "./shadow-maps/cube-depth.fs"
  ),
  mp_Instanced_cshadow(
    "./shaders/lighting.vs",
    "./shadow-maps/cube-depth.gs",
    "./shadow-maps/cube-depth.fs",
    { "#define INSTANCED\n" }
  ),
#ifdef POINTLIGHT_DEBUG
  mp_Plain_cshadow(
    "./shaders/plain.vs", "./shadow-maps/cube-depth.gs", "./shadow-maps/cube-depth.fs"
  ),
#endif
  mp_ReflectSkybox_cshadow(
    "./playground/skybox/reflect-skybox.vs",
    "./shadow-maps/cube-depth.gs",
    "./shadow-maps/cube-depth.fs"
  ),
  mp_Floor_cshadow(
    "./disco/floor.vs", "./shadow-maps/cube-depth.gs", "./shadow-maps/cube-depth.fs"
  ),

  proj_x_view_ub(
    0,
    {
#ifdef DEFERRED
      mpGSkeletal,
      mpGSkeletalNormal,
      mpGNormalHeight,
      mpGInstanced,
      mpGLighting,
#else
      mpSkeletal,
      mpSkeletalNormal,
      mpNormalHeight,
      mpInstanced,
#endif
#ifdef POINTLIGHT_DEBUG
      mpPlain,
#endif
#ifdef NORMALS_DEBUG
      mpNormals,
#endif
      mpReflectSkybox,
      mpLighting,
      mpFloor,

      mp_Skeletal_shadow,
      mp_Lighting_shadow,
      mp_Instanced_shadow,
#ifdef POINTLIGHT_DEBUG
      mp_Plain_shadow,
#endif
      mp_ReflectSkybox_shadow,
      mp_Floor_shadow,

      mp_Skeletal_cshadow,
      mp_Lighting_cshadow,
      mp_Instanced_cshadow,
#ifdef POINTLIGHT_DEBUG
      mp_Plain_cshadow,
#endif
      mp_ReflectSkybox_cshadow,
      mp_Floor_cshadow,
    }
  ),
  lightingSettings(
    1,
    {
#ifdef DEFERRED
      mpDeferredLighting,
#else
      mpSkeletal,
      mpSkeletalNormal,
      mpNormalHeight,
      mpInstanced,
#endif
      mpLighting,
      mpFloor,
    },
    NUM_DIR_LIGHTS,
    NUM_POINT_LIGHTS,
    NUM_SPOT_LIGHTS
  ),
#ifdef BLOOM
  mpBrightness("./shaders/postprocess/plain.vs", nullptr, "./shaders/postprocess/brightness.fs"),
  mpGaussian("./shaders/postprocess/plain.vs", nullptr, "./shaders/postprocess/gaussian.fs"),
  mGaussianBuffer(1, RenderBuffer::Format::RGB16F, 800, 600, 2),
#endif
#ifdef HDR
  mpBloom("./shaders/postprocess/plain.vs", nullptr, "./shaders/postprocess/bloom.fs"),
  mHDRBuffer(SAMPLES, RenderBuffer::Format::RGBA16F, 800, 600, 1),
#endif
  mShadows(NUM_DIR_LIGHTS, NUM_POINT_LIGHTS, NUM_SPOT_LIGHTS)
{
  mLastFrame = 0.f;
  mLastX = 400;
  mLastY = 300;
  mFirstMouse = true;
  mSpotlightOn = false;
  mExposure = 0.f;
  addExposure(0.7f);
  mBloomOn = false;
  toggleBloom();
}

Playground::~Playground() { }

void Playground::setup()
{
  glm::mat4 modelDae = glm::mat4(1.f);
  modelDae = glm::rotate(modelDae, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  modelDae = glm::scale(modelDae, glm::vec3(0.25f));
  dae.setModel(modelDae);

  glm::mat4 modelTifa = glm::mat4(1.f);
  modelTifa = glm::translate(modelTifa, glm::vec3(-1.5f, 0.f, 0.f));
  tifa.setModel(modelTifa);

  glm::mat4 modelWhipper = glm::mat4(1.f);
  modelWhipper = glm::translate(modelWhipper, glm::vec3(1.5f, 0.f, 0.f));
  modelWhipper = glm::rotate(modelWhipper, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  modelWhipper = glm::scale(modelWhipper, glm::vec3(0.15f));
  whipper.setModel(modelWhipper);

  glm::mat4 modelIcarus = glm::mat4(1.f);
  modelIcarus = glm::translate(modelIcarus, glm::vec3(3.f, 0.f, 0.f));

  // modelIcarus = glm::translate(modelIcarus, glm::vec3(3.f, 0.5f, 0.f));
  // modelIcarus = glm::rotate(modelIcarus, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
  modelIcarus = glm::scale(modelIcarus, glm::vec3(0.010f));
  icarus.setModel(modelIcarus);

  // glm::vec3 n = glm::vec3(1.0, 0.765, 0.123);
  // glm::vec3 a = glm::normalize(glm::mat3(transpose(inverse(modelDae))) *
  // glm::mat3(modelWhipper)
  // * n); glm::vec3 b = glm::normalize(glm::mat3(transpose(inverse(modelDae * modelWhipper))) *
  // n);

  // std::cout << a.r << b.r;

  glm::mat4 mirrorModel = glm::mat4(1.0);
  mirrorModel = glm::translate(mirrorModel, glm::vec3(2.5f, 1.5f, -1.f));
  mirrorModel = glm::rotate(mirrorModel, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
  mirrorModel = glm::scale(mirrorModel, glm::vec3(0.3f));
  mirror.setModel(mirrorModel);

  floor.setModel(glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f)));

  window.setModel(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 2.f, -1.f)));

  wall.setModel(glm::translate(glm::mat4(1.f), glm::vec3(4.5f, 1.f, -0.5f)));
  toy.setModel(glm::translate(glm::mat4(1.f), glm::vec3(4.5f, 3.f, -0.5f)));

  glm::vec3 cameraPos = glm::vec3(0.f, 1.f, 8.f);
  glm::vec3 pointTo = glm::vec3(0.f, 1.f, 0.f);

#ifdef FOLLOW_WHIPPER
  const glm::vec3& whipperPos = whipper.position();
  cameraPos = whipperPos + glm::vec3(0.f, 1.f, 8.f);
  pointTo = whipperPos + glm::vec3(0.f, 1.f, 0.f);
#endif

  const bool animate = false;
  mCamera.setPosition(cameraPos, animate);
  mCamera.pointTo(pointTo, animate);

  std::vector<DirLight> dirLights = lightingSettings.getDirLights(NUM_DIR_LIGHTS);
  std::vector<glm::mat4> light_spaces;
  for (DirLight dirLight : dirLights)
    light_spaces.push_back(
      mShadows.light_space(-dirLight.direction * DIR_LIGHT_DISTANCE, glm::vec3(0.f))
    );
  unsigned int first;

#if (NUM_DIR_LIGHTS > 0) or (NUM_POINT_LIGHTS > 0)
  mShadows.bindTextures();
#endif

#define SHININESS 32.f

#ifdef DEFERRED
  mpDeferredLighting.use();
  mpDeferredLighting.setFloat("u_shininess", SHININESS);

  // draw dir shadow map
  for (unsigned int i = 0; i < light_spaces.size(); i++) {
    mpDeferredLighting.setMat4fv(
      ("u_light_space[" + std::to_string(i) + "]").c_str(), light_spaces[i]
    );
  }
  first = LOCATIONS::TEXTURES::SHADOWMAPS0;
  for (unsigned int i = 0; i < NUM_DIR_LIGHTS; i++) {
    mpDeferredLighting.setSampler(
      ("u_dir_shadow_maps[" + std::to_string(i) + "]").c_str(), i + first
    );
  }

  // draw cube shadow map
  mpDeferredLighting.setFloat("u_far", CUBE_FAR);
  first += NUM_DIR_LIGHTS;
  for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++) {
    mpDeferredLighting.setSampler(
      ("u_point_shadow_maps[" + std::to_string(i) + "]").c_str(), i + first
    );
  }

  mpGNormalHeight.use();
  mpGNormalHeight.setFloat("u_height_scale", 0.15);
#else
  mpSkeletal.use();
  mpSkeletal.setFloat("u_material.shininess", SHININESS);

  mpSkeletalNormal.use();
  mpSkeletalNormal.setFloat("u_material.shininess", SHININESS);
  mpSkeletalNormal.setVec4fv("u_material.specular_color", Color { 0.5f });

  mpNormalHeight.use();
  mpNormalHeight.setFloat("u_material.shininess", SHININESS);
  mpNormalHeight.setFloat("u_height_scale", 0.15);
  for (unsigned int i = 0; i < light_spaces.size(); i++) {
    mpNormalHeight.setMat4fv(("u_light_space[" + std::to_string(i) + "]").c_str(), light_spaces[i]);
  }
  first = LOCATIONS::TEXTURES::SHADOWMAPS0;
  for (unsigned int i = 0; i < NUM_DIR_LIGHTS; i++) {
    mpNormalHeight.setSampler(("u_dir_shadow_maps[" + std::to_string(i) + "]").c_str(), i + first);
  }

  mpNormalHeight.setFloat("u_far", CUBE_FAR);
  first += NUM_DIR_LIGHTS;
  for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++) {
    mpNormalHeight.setSampler(
      ("u_point_shadow_maps[" + std::to_string(i) + "]").c_str(), i + first
    );
  }

  mpInstanced.use();
  mpInstanced.setFloat("u_material.shininess", SHININESS);

  for (unsigned int i = 0; i < light_spaces.size(); i++) {
    mpInstanced.setMat4fv(("u_light_space[" + std::to_string(i) + "]").c_str(), light_spaces[i]);
  }
  first = LOCATIONS::TEXTURES::SHADOWMAPS0;
  for (unsigned int i = 0; i < NUM_DIR_LIGHTS; i++) {
    mpInstanced.setSampler(("u_dir_shadow_maps[" + std::to_string(i) + "]").c_str(), i + first);
  }

  mpInstanced.setFloat("u_far", CUBE_FAR);
  first += NUM_DIR_LIGHTS;
  for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++) {
    mpInstanced.setSampler(("u_point_shadow_maps[" + std::to_string(i) + "]").c_str(), i + first);
  }
#endif

// mpLighting.setFloat("u_time", -M_PI_2);
#ifdef NORMALS_DEBUG
  mpNormals.use();
  mpNormals.setVec4fv("u_color", glm::vec4(1.f, 0.f, 1.f, 1.f));
#endif

  mpLighting.use();
  mpLighting.setFloat("u_material.shininess", SHININESS);

  mpFloor.use();
  mpFloor.setFloat("u_material.shininess", SHININESS);
  for (unsigned int i = 0; i < light_spaces.size(); i++) {
    mpFloor.setMat4fv(("u_light_space[" + std::to_string(i) + "]").c_str(), light_spaces[i]);
  }
  first = LOCATIONS::TEXTURES::SHADOWMAPS0;
  for (unsigned int i = 0; i < NUM_DIR_LIGHTS; i++) {
    mpFloor.setSampler(("u_dir_shadow_maps[" + std::to_string(i) + "]").c_str(), i + first);
  }
  mpFloor.setFloat("u_far", CUBE_FAR);
  first += NUM_DIR_LIGHTS;
  for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++) {
    mpFloor.setSampler(("u_point_shadow_maps[" + std::to_string(i) + "]").c_str(), i + first);
  }
}

void Playground::update(const float& time)
{
  const bool animatingPos = mCamera.updatePosition(time);
  const bool animatingDir = mCamera.updateDirection(time);

  tifa.update(time);
  dae.update(time);
  whipper.update(time);
  floor.update(time);
  // icarus.update(time);

  if (animatingPos || animatingDir) {
    return;
  }
#ifdef FOLLOW_WHIPPER
  const glm::vec3& whipperPos = whipper.position();
  mCamera.setPosition(whipperPos + glm::vec3(0.f, 1.f, 8.f));
  mCamera.pointTo(whipperPos + glm::vec3(0.f, 1.f, 0.f));
#endif

#if NUM_DIR_LIGHTS > 0
  std::vector<glm::vec3> dirLightPos;
  std::vector<glm::vec3> dirLightPointTo;
  std::vector<DirLight> dirLights = lightingSettings.getDirLights(NUM_DIR_LIGHTS);
  for (DirLight dirLight : dirLights) {
    dirLightPos.push_back(-dirLight.direction * DIR_LIGHT_DISTANCE);
    dirLightPointTo.push_back(glm::vec3(0.0));
  }
  mShadows.updateDirMaps(*this, dirLightPos, dirLightPointTo);
#endif
#if NUM_POINT_LIGHTS > 0
  lightingSettings.setPointLight0Position(
    glm::vec2(1.0f + sin(time) * 2.0f, sin(time / 2.0f) * 1.0f + 3.f)
  );

  std::vector<PointLight> pointLights = lightingSettings.getPointLights(NUM_POINT_LIGHTS);
  std::vector<glm::vec3> pointLightPos;
  for (PointLight pointLight : pointLights)
    pointLightPos.push_back(pointLight.position);
  mShadows.updateCubeMaps(*this, pointLightPos, CUBE_FAR);
#endif
  mirror.screenshot(*this);
}

void Playground::render() const
{
#ifdef HDR
  // lighting
  render(mHDRBuffer.fbo(), this->camera());
  mHDRBuffer.blit();

#ifdef BLOOM
  // brightness
  glBindFramebuffer(GL_FRAMEBUFFER, mGaussianBuffer.fbo());
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mpBrightness.use();
  mpBrightness.setSampler("tex", 0);
  mDrawTexture.render(mHDRBuffer.textures(0), 1);

  // gaussian / blur
  const unsigned int PASSES = 10;

  mpGaussian.use();
  mpGaussian.setSampler("tex", 0);

  // brightness / initial value in GL_COLOR_ATTACHMENT0, start with GL_COLOR_ATTACHMENT1
  bool horizontal = true;
  for (unsigned int i = 0; i < PASSES; i++) {
    mpGaussian.setBool("horizontal", horizontal);

    glDrawBuffer(GL_COLOR_ATTACHMENT0 + horizontal);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mDrawTexture.render(mGaussianBuffer.textures(!horizontal));

    horizontal = !horizontal;
  }
#endif

  // draw
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  mpBloom.use();
  mpBloom.setFloat("u_exposure", mExposure);
  mpBloom.setSampler("scene", 0);
#ifdef BLOOM
  mpBloom.setSampler("bloom", 1);
  mpBloom.setBool("u_bloom_on", mBloomOn);
#else
  mpBloom.setBool("u_bloom_on", false);
#endif

  const Texture* scene = mHDRBuffer.textures(0);
#ifdef BLOOM
  const Texture* bloom = mGaussianBuffer.textures(0);
  const Texture texs[2] = { *scene, *bloom };
  mDrawTexture.render(texs, 2);
#else
  mDrawTexture.render(scene, 1);
#endif
// mDrawTexture.render(&texs[1], 1);
#else
  render(camera());
#endif
}

void Playground::render(const unsigned int fbo, const Camera& camera) const
{
  const Shader* shaders[10];

#ifdef DEFERRED
  shaders[ShadersIndex::ISKELETAL] = &mpGSkeletal;
  shaders[ShadersIndex::ISKELETAL_NORMAL_MAP] = &mpGSkeletalNormal;
  shaders[ShadersIndex::INORMAL_HEIGHT] = &mpGNormalHeight;
  shaders[ShadersIndex::IINSTANCED] = &mpGInstanced;
  shaders[ShadersIndex::ILIGHTING_DEFF] = &mpGLighting;
#else
  shaders[ShadersIndex::ISKELETAL] = &mpSkeletal;
  shaders[ShadersIndex::ISKELETAL_NORMAL_MAP] = &mpSkeletalNormal;
  shaders[ShadersIndex::INORMAL_HEIGHT] = &mpNormalHeight;
  shaders[ShadersIndex::IINSTANCED] = &mpInstanced;
  shaders[ShadersIndex::ILIGHTING_DEFF] = &mpLighting;
#endif
#ifdef POINTLIGHT_DEBUG
  shaders[ShadersIndex::IPLAIN] = &mpPlain;
#endif
#ifdef NORMALS_DEBUG
  shaders[ShadersIndex::INORMALS] = &mpNormals;
#endif
  shaders[ShadersIndex::IREFLECT_SKYBOX] = &mpReflectSkybox;
  shaders[ShadersIndex::ILIGHTING] = &mpLighting;
  shaders[ShadersIndex::IFLOOR] = &mpFloor;
  shaders[ShadersIndex::ISKYBOX] = &mpSkybox;

#ifdef DEFERRED
  glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.fbo());
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const glm::mat4& projection = camera.projection();
  const glm::mat4& view = camera.view();
  const glm::vec3& view_pos = camera.position();
  const glm::vec3& view_dir = camera.front();
  const glm::mat4 proj_x_view = projection * view;

  lightingSettings.updatePointLight0Position();
  lightingSettings.updateSpotLight(view_pos, view_dir, !mSpotlightOn);
  proj_x_view_ub.set(proj_x_view);

  renderDeffered(projection, view, view_pos, view_dir, shaders);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // mpBloom.use();
  // mpBloom.setFloat("u_exposure", mExposure);
  // mpBloom.setSampler("scene", 0);
  // mpBloom.setBool("u_bloom_on", false);
  // mDrawTexture.render(gBuffer.textures(4), 1);
  // return;

  mpDeferredLighting.use();
  mpDeferredLighting.setSampler("in_position", 0);
  mpDeferredLighting.setSampler("in_normal", 1);
  mpDeferredLighting.setSampler("in_color", 2);
  mpDeferredLighting.setSampler("in_alpha", 3);
  mpDeferredLighting.setSampler("in_specular", 4);
  mpDeferredLighting.setSampler("in_emissive", 5);
  mpDeferredLighting.setSampler("in_depth", 6);
  mpDeferredLighting.setVec3fv("u_view_dir", view_dir);

  mDrawTexture.render(gBuffer.textures(), gBuffer.count());

  // glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer.fbo());
  // glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  // glBlitFramebuffer(0, 0, 800.f, 600.f, 0, 0, 800.f, 600.f, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

  renderForward(projection, view, view_pos, view_dir, shaders);

#else

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderScene(camera.projection(), camera.view(), camera.position(), camera.front(), shaders);
#endif
}

void Playground::renderShadowMap(
  const unsigned int fbo, const glm::mat4& projection, const glm::mat4& view
) const
{
  const Shader* shaders[10];
  shaders[ShadersIndex::ISKELETAL] = &mp_Skeletal_shadow;
  shaders[ShadersIndex::ISKELETAL_NORMAL_MAP] = &mp_Skeletal_shadow;
  shaders[ShadersIndex::INORMAL_HEIGHT] = &mp_Lighting_shadow;
  shaders[ShadersIndex::IINSTANCED] = &mp_Instanced_shadow;
#ifdef POINTLIGHT_DEBUG
  shaders[ShadersIndex::IPLAIN] = &mp_Plain_shadow;
#endif
#ifdef NORMALS_DEBUG
  shaders[ShadersIndex::INORMALS] = nullptr;
#endif
  shaders[ShadersIndex::IREFLECT_SKYBOX] = &mp_ReflectSkybox_shadow;
  shaders[ShadersIndex::ILIGHTING_DEFF] = &mp_Lighting_shadow;
  shaders[ShadersIndex::ILIGHTING] = &mp_Lighting_shadow;
  shaders[ShadersIndex::IFLOOR] = &mp_Floor_shadow;
  shaders[ShadersIndex::ISKYBOX] = nullptr;

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderScene(projection, view, glm::vec3(0), glm::vec3(0), shaders);
}

void Playground::renderCubeMap(
  const unsigned int fbo,
  const std::vector<glm::mat4>& cube_space,
  const glm::vec3& lightPos,
  const float& far
) const
{
  const Shader* shaders[10];
  shaders[ShadersIndex::ISKELETAL] = &mp_Skeletal_cshadow;
  shaders[ShadersIndex::ISKELETAL_NORMAL_MAP] = &mp_Skeletal_cshadow;
  shaders[ShadersIndex::INORMAL_HEIGHT] = &mp_Lighting_cshadow;
  shaders[ShadersIndex::IINSTANCED] = &mp_Instanced_cshadow;
#ifdef POINTLIGHT_DEBUG
  shaders[ShadersIndex::IPLAIN] = &mp_Plain_cshadow;
#endif
#ifdef NORMALS_DEBUG
  shaders[ShadersIndex::INORMALS] = nullptr;
#endif
  shaders[ShadersIndex::IREFLECT_SKYBOX] = &mp_ReflectSkybox_cshadow;
  shaders[ShadersIndex::ILIGHTING_DEFF] = &mp_Lighting_cshadow;
  shaders[ShadersIndex::ILIGHTING] = &mp_Lighting_cshadow;
  shaders[ShadersIndex::IFLOOR] = &mp_Floor_cshadow;
  shaders[ShadersIndex::ISKYBOX] = nullptr;

  for (unsigned int j = 0; j < 8; j++) {
    const Shader* s = shaders[j];
    if (s != nullptr) {
      s->use();
      for (unsigned int i = 0; i < 6; i++)
        s->setMat4fv(("u_cube_space[" + std::to_string(i) + "]").c_str(), cube_space[i]);

      s->setVec3fv("u_light_pos", lightPos);
      s->setFloat("u_far", far);
    }
  }

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderScene(glm::mat4(1.0), glm::mat4(1.0), glm::vec3(0), glm::vec3(0), shaders);
}

void Playground::renderScene(
  const glm::mat4& projection,
  const glm::mat4& view,
  const glm::vec3& view_pos,
  const glm::vec3& view_dir,
  const Shader** shaders
) const
{
  const glm::mat4 proj_x_view = projection * view;

  lightingSettings.updatePointLight0Position();
  lightingSettings.updateSpotLight(view_pos, view_dir, !mSpotlightOn);
  proj_x_view_ub.set(proj_x_view);

  renderDeffered(projection, view, view_pos, view_dir, shaders);
  renderForward(projection, view, view_pos, view_dir, shaders);
}

void Playground::renderDeffered(
  const glm::mat4& projection,
  const glm::mat4& view,
  const glm::vec3& view_pos,
  const glm::vec3& view_dir,
  const Shader** shaders
) const
{
  const Shader& p_skeletal = *shaders[ShadersIndex::ISKELETAL];
  const Shader& p_skeletal_normal_map = *shaders[ShadersIndex::ISKELETAL_NORMAL_MAP];
  const Shader& p_normal_map = *shaders[ShadersIndex::INORMAL_HEIGHT];
  const Shader& p_instanced = *shaders[ShadersIndex::IINSTANCED];
  // const Shader* p_plain = shaders[ShadersIndex::IPLAIN];
  // const Shader* p_normals = shaders[ShadersIndex::INORMALS];
  // const Shader& p_reflect_skybox = *shaders[ShadersIndex::IREFLECT_SKYBOX];
  const Shader& p_lighting_deff = *shaders[ShadersIndex::ILIGHTING_DEFF];
  // const Shader& p_lighting = *shaders[ShadersIndex::ILIGHTING];
  // const Shader& p_floor = *shaders[ShadersIndex::IFLOOR];
  // const Shader* p_skybox = shaders[ShadersIndex::ISKYBOX];

  p_skeletal.use();
  p_skeletal.setVec3fv("u_view_pos", view_pos);

  // tifa
  tifa.render(p_skeletal);

  // dae
  dae.render(p_skeletal);

  // whipper
  whipper.render(p_skeletal);

  p_skeletal_normal_map.use();
  p_skeletal_normal_map.setVec3fv("u_view_pos", view_pos);

  // icarus
  icarus.render(p_skeletal_normal_map);

  p_normal_map.use();
  p_normal_map.setVec3fv("u_view_pos", view_pos);

  wall.render(p_normal_map);
  toy.render(p_normal_map);

  p_instanced.use();
  p_instanced.setVec3fv("u_view_pos", view_pos);

  box.render(p_instanced);
  grass.render(p_instanced);

  p_lighting_deff.use();
  p_lighting_deff.setVec3fv("u_view_pos", view_pos);

  glm::mat4 model2b = glm::mat4(1.f);
  model2b = glm::translate(model2b, glm::vec3(0.f, 1.75f, 0.f));
  model2b = glm::rotate(model2b, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
  p_lighting_deff.setMat4fv("u_model", model2b);
  p_lighting_deff.setMat3fv(
    "u_trans_inver_model", glm::mat3(glm::transpose(glm::inverse(model2b)))
  );
  simpleModel.draw(p_lighting_deff);

  mirror.render(p_lighting_deff);
}
void Playground::renderForward(
  const glm::mat4& projection,
  const glm::mat4& view,
  const glm::vec3& view_pos,
  const glm::vec3& view_dir,
  const Shader** shaders
) const
{
  // const Shader& p_skeletal = *shaders[ShadersIndex::ISKELETAL];
  // const Shader& p_skeletal_normal_map = *shaders[ShadersIndex::ISKELETAL_NORMAL_MAP];
  // const Shader& p_normal_map = *shaders[ShadersIndex::INORMAL_HEIGHT];
// const Shader& p_instanced = *shaders[ShadersIndex::IINSTANCED];
#ifdef POINTLIGHT_DEBUG
  const Shader* p_plain = shaders[ShadersIndex::IPLAIN];
#endif
#ifdef NORMALS_DEBUG
  const Shader* p_normals = shaders[ShadersIndex::INORMALS];
#endif
  const Shader& p_reflect_skybox = *shaders[ShadersIndex::IREFLECT_SKYBOX];
  // const Shader& p_lighting_deff = *shaders[ShadersIndex::ILIGHTING_DEFF];
  const Shader& p_lighting = *shaders[ShadersIndex::ILIGHTING];
  const Shader& p_floor = *shaders[ShadersIndex::IFLOOR];
  const Shader* p_skybox = shaders[ShadersIndex::ISKYBOX];

#ifdef POINTLIGHT_DEBUG
  if (p_plain != nullptr) {
    p_plain->use();
    std::vector<PointLight> pointLights = lightingSettings.getPointLights(NUM_POINT_LIGHTS);
    for (PointLight pointLight : pointLights) {
      glm::mat4 lightModel = glm::translate(glm::mat4(1.0), pointLight.position);
      lightModel = glm::scale(lightModel, glm::vec3(0.2f));

      p_plain->setVec4fv("u_color", pointLight.light.color.diffuse);
      p_plain->setMat4fv("u_model", lightModel);
      pointLightDebug.render(*p_plain);
    }
    p_plain->setVec4fv("u_color", glm::vec4(0));
  }
#endif

#ifdef NORMALS_DEBUG
  if (p_normals != nullptr) {
    p_normals->use();
    box.render(*p_normals);
  }
#endif

  p_reflect_skybox.use();
  p_reflect_skybox.setVec3fv("u_view_pos", view_pos);
  skyboxReflector.draw(p_reflect_skybox);

  // draw last (late as possible) furthest/z-buffer
  if (p_skybox != nullptr) {
    p_skybox->use();
    // remove translation
    const glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
    p_skybox->setMat4fv("u_proj_x_view", projection * skyboxView);
    skybox.draw(*p_skybox);
  }

  // draw last (late as possible) translucent / blended
  p_lighting.use();
  p_lighting.setVec3fv("u_view_pos", view_pos);
  window.render(p_lighting);

  // draw last (late as possible) translucent / blended
  p_floor.use();
  p_floor.setVec3fv("u_view_pos", view_pos);
  floor.render(p_floor);
}

void Playground::teardown()
{
  tifa.free();
  dae.free();
  whipper.free();

  icarus.free();

  simpleModel.free();
  grass.free();
  window.free();
  mirror.free();

  wall.free();
  toy.free();

  floor.free();

  box.free();

  skybox.free();

#ifdef POINTLIGHT_DEBUG
  pointLightDebug.free();
#endif

  skyboxReflector.free();

#ifdef DEFERRED
  gBuffer.free();
  mpDeferredLighting.free();
  mpGSkeletal.free();
  mpGSkeletalNormal.free();
  mpGNormalHeight.free();
  mpGInstanced.free();
  mpGLighting.free();
#else
  mpSkeletal.free();
  mpSkeletalNormal.free();
  mpNormalHeight.free();
  mpInstanced.free();
#endif
#ifdef POINTLIGHT_DEBUG
  mpPlain.free();
#endif
#ifdef NORMALS_DEBUG
  mpNormals.free();
#endif
  mpReflectSkybox.free();
  mpLighting.free();
  mpFloor.free();
  mpSkybox.free();

  mp_Skeletal_shadow.free();
  mp_Lighting_shadow.free();
  mp_Instanced_shadow.free();
#ifdef POINTLIGHT_DEBUG
  mp_Plain_shadow.free();
#endif
  mp_ReflectSkybox_shadow.free();
  mp_Floor_shadow.free();

  mp_Skeletal_cshadow.free();
  mp_Lighting_cshadow.free();
  mp_Instanced_cshadow.free();
#ifdef POINTLIGHT_DEBUG
  mp_Plain_cshadow.free();
#endif
  mp_ReflectSkybox_cshadow.free();
  mp_Floor_cshadow.free();

  lightingSettings.free();
  proj_x_view_ub.free();

#if (NUM_DIR_LIGHTS > 0) or (NUM_POINT_LIGHTS > 0)
  mShadows.unbindTextures();
#endif

#ifdef BLOOM
  mpBrightness.free();
  mpGaussian.free();
  mGaussianBuffer.free();
#endif
#ifdef HDR
  mpBloom.free();
  mDrawTexture.free();
  mHDRBuffer.free();
#endif

  mShadows.free();
}

void Playground::highlight(Box& box, glm::mat4 model)
{
  // glEnable(GL_STENCIL_TEST);
  // glClear(GL_STENCIL_BUFFER_BIT);

  // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  // glStencilFunc(GL_ALWAYS, 1, 0xFF);
  // glStencilMask(0xFF);

  // mpLighting.use();
  // mpLighting.setMat4fv("u_model", model);
  // box.draw(mpLighting);

  // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  // glStencilMask(0x00);
  // // glDisable(GL_DEPTH_TEST); ?
  // mpPlain.use();
  // mpPlain.setVec4fv("u_color", glm::vec4(1.0, 1.0, 1.0, 1.0));
  // mpPlain.setMat4fv("u_model", glm::scale(model, glm::vec3(1.1)));
  // box.draw(mpPlain);

  // glStencilMask(0xFF);
  // glStencilFunc(GL_ALWAYS, 1, 0xFF);
  // // glEnable(GL_DEPTH_TEST);

  // glDisable(GL_STENCIL_TEST);
}

void Playground::handleInput(const GLFWwindow* window)
{
#ifdef FOLLOW_WHIPPER
  Scene scene = (Scene) * this;
  whipper.handleInput(window, scene);
#else
  const float time = glfwGetTime();
  const float deltaTime = time - mLastFrame;
  mLastFrame = time;

  if (glfwGetKey((GLFWwindow*)window, GLFW_KEY_W) == GLFW_PRESS)
    mCamera.moveForward(2.5f * deltaTime);
  else if (glfwGetKey((GLFWwindow*)window, GLFW_KEY_S) == GLFW_PRESS)
    mCamera.moveBackward(2.5f * deltaTime);
  else if (glfwGetKey((GLFWwindow*)window, GLFW_KEY_A) == GLFW_PRESS)
    mCamera.moveLeft(2.5f * deltaTime);
  else if (glfwGetKey((GLFWwindow*)window, GLFW_KEY_D) == GLFW_PRESS)
    mCamera.moveRight(2.5f * deltaTime);
#ifdef HDR
  else if (glfwGetKey((GLFWwindow*)window, GLFW_KEY_E) == GLFW_PRESS) {
    addExposure(0.5f * deltaTime);
#endif
  }

#endif
}

void Playground::onChar(const char c)
{
  if (c == 'f') {
    toggleSpotLight();
#ifdef BLOOM
  } else if (c == 'b') {
    toggleBloom();
#endif
  } else if (c == ' ') {
    std::cout << " ";
  }
}

void Playground::onMouse(const GLFWwindow* window, const double x, const double y)
{
  if (mFirstMouse) {
    mLastX = x;
    mLastY = y;
    mFirstMouse = false;
  }

  const float xoffset = x - mLastX;
  const float yoffset = y - mLastY;
  mLastX = x;
  mLastY = y;

  if (glfwGetMouseButton((GLFWwindow*)window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    mCamera.changeDirection(xoffset, yoffset);
  }
}

void Playground::onScroll(const GLFWwindow* window, const double x, const double y)
{
  mCamera.zoom((float)y);
}
