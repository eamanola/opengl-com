#include "playground.h"
#include "shaders/locations.h"

// #define FOLLOW_WHIPPER

#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 4
#define NUM_SPOT_LIGHTS 1

#define DIR_LIGHT_DISTANCE 5.f
#define CUBE_FAR 15.f

Playground::Playground() :
  mpSkeletal(
    "./skeletal/skeletal.vs",
    nullptr,
    "./shaders/lighting.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
  mp_Skeletal_shadow("./skeletal/skeletal.vs", nullptr, "./shadow-maps/simple-depth.fs"),
  mp_Skeletal_cshadow(
    "./skeletal/skeletal.vs", "./shadow-maps/cube-depth.gs", "./shadow-maps/cube-depth.fs"
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
  mp_Lighting_shadow(
    "./shaders/lighting.vs", nullptr, "./shadow-maps/simple-depth.fs", { "#define FRAG_POS\n" }
  ),
  mp_Lighting_cshadow(
    "./shaders/lighting.vs",
    "./shadow-maps/cube-depth.gs",
    "./shadow-maps/cube-depth.fs",
    { "#define FRAG_POS\n" }
  ),
  simpleModel("assets/2b-jumps2/scene.gltf"),
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
  mp_Floor_shadow("./disco/floor.vs", nullptr, "./shadow-maps/simple-depth.fs"),
  mp_Floor_cshadow(
    "./disco/floor.vs", "./shadow-maps/cube-depth.gs", "./shadow-maps/cube-depth.fs"
  ),
  floor(25, 25),
  mpInstanced(
    "./shaders/lighting-instanced.vs",
    nullptr,
    "./shaders/lighting.fs",
    {
      "#define IN_NR_DIR_LIGHTS " + std::to_string(NUM_DIR_LIGHTS) + "\n",
      "#define IN_NR_POINT_LIGHTS " + std::to_string(NUM_POINT_LIGHTS) + "\n",
      "#define IN_NR_SPOT_LIGHTS " + std::to_string(NUM_SPOT_LIGHTS) + "\n",
      "#define IN_RENDER_SHADOWS\n",
    },
    { "shaders/lighted-shader-defines" }
  ),
  mp_Instanced_shadow(
    "./shaders/lighting-instanced.vs",
    nullptr,
    "./shadow-maps/simple-depth.fs",
    { "#define FRAG_POS\n" }
  ),
  mp_Instanced_cshadow(
    "./shaders/lighting-instanced.vs",
    "./shadow-maps/cube-depth.gs",
    "./shadow-maps/cube-depth.fs",
    { "#define FRAG_POS\n" }
  ),
  mpSkybox("./playground/skybox/cube.vs", nullptr, "./playground/skybox/cube.fs"),
  mpReflectSkybox(
    "./playground/skybox/reflect-skybox.vs", nullptr, "./playground/skybox/reflect-skybox.fs"
  ),
  mp_ReflectSkybox_shadow(
    "./playground/skybox/reflect-skybox.vs", nullptr, "./shadow-maps/simple-depth.fs"
  ),
  mp_ReflectSkybox_cshadow(
    "./playground/skybox/reflect-skybox.vs",
    "./shadow-maps/cube-depth.gs",
    "./shadow-maps/cube-depth.fs"
  ),
#ifdef SHADOW_DEBUG
  mpShadowsDebug("./shadow-maps/shadows-debug.vs", nullptr, "./shadow-maps/shadows-debug.fs"),
#endif
#ifdef POINTLIGHT_DEBUG
  mpPlain("./shaders/plain.vs", nullptr, "./shaders/single-color.fs"),
  mp_Plain_shadow("./shaders/plain.vs", nullptr, "./shadow-maps/simple-depth.fs"),
  mp_Plain_cshadow(
    "./shaders/plain.vs", "./shadow-maps/cube-depth.gs", "./shadow-maps/cube-depth.fs"
  ),
#endif
#ifdef NORMALS_DEBUG
  mpNormals(
    "./shaders/lighting-instanced.vs",
    "./shaders/draw-normals.gs",
    "./shaders/single-color.fs",
    { "#define NORMAL\n" }
  ),
#endif
  lightingSettings(
    1,
    { mpSkeletal, mpLighting, mpFloor, mpInstanced },
    NUM_DIR_LIGHTS,
    NUM_POINT_LIGHTS,
    NUM_SPOT_LIGHTS
  ),
  mSpotlightOn(false),
  proj_x_view_ub(
    0,
    {
      mpSkeletal,      mp_Skeletal_shadow,      mp_Skeletal_cshadow,
      mpLighting,      mp_Lighting_shadow,      mp_Lighting_cshadow,
      mpFloor,         mp_Floor_shadow,         mp_Floor_cshadow,
      mpInstanced,     mp_Instanced_shadow,     mp_Instanced_cshadow,
      mpReflectSkybox, mp_ReflectSkybox_shadow, mp_ReflectSkybox_cshadow,
#ifdef POINTLIGHT_DEBUG
      mpPlain,         mp_Plain_shadow,         mp_Plain_cshadow,
#endif
#ifdef NORMALS_DEBUG
      mpNormals,
#endif
    }
  ),
  mShadows(NUM_DIR_LIGHTS, NUM_POINT_LIGHTS, NUM_SPOT_LIGHTS)
{
  mLastFrame = 0.f;
  mLastX = 400;
  mLastY = 300;
  mFirstMouse = true;
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

  // glm::vec3 n = glm::vec3(1.0, 0.765, 0.123);
  // glm::vec3 a = glm::normalize(glm::mat3(transpose(inverse(modelDae))) * glm::mat3(modelWhipper)
  // * n); glm::vec3 b = glm::normalize(glm::mat3(transpose(inverse(modelDae * modelWhipper))) * n);

  // std::cout << a.r << b.r;

  glm::mat4 mirrorModel = glm::mat4(1.0);
  mirrorModel = glm::translate(mirrorModel, glm::vec3(2.5f, 1.5f, -1.f));
  mirrorModel = glm::rotate(mirrorModel, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
  mirrorModel = glm::scale(mirrorModel, glm::vec3(0.3f));
  mirror.setModel(mirrorModel);

  floor.setModel(glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f)));

  window.setModel(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1.f, -1.f)));

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

#define SHININESS 32.f
  mpSkeletal.use();
  mpSkeletal.setFloat("u_material.shininess", SHININESS);

  mpLighting.use();
  mpLighting.setFloat("u_material.shininess", SHININESS);

  mpFloor.use();
  mpFloor.setFloat("u_material.shininess", SHININESS);
  // draw dir shadow map
  for (unsigned int i = 0; i < light_spaces.size(); i++) {
    mpFloor.setMat4fv(("u_light_space[" + std::to_string(i) + "]").c_str(), light_spaces[i]);
  }
  // draw cube shadow map
  mpFloor.setFloat("u_far", CUBE_FAR);
  unsigned int first = LOCATIONS::TEXTURES::SHADOWMAPS0;
  for (unsigned int i = 0; i < NUM_DIR_LIGHTS; i++) {
    mpFloor.setInt(("u_dir_shadow_maps[" + std::to_string(i) + "]").c_str(), i + first);
  }
  first += NUM_DIR_LIGHTS;
  for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++) {
    mpFloor.setInt(("u_point_shadow_maps[" + std::to_string(i) + "]").c_str(), i + first);
  }

  mpInstanced.use();
  mpInstanced.setFloat("u_material.shininess", SHININESS);
  for (unsigned int i = 0; i < light_spaces.size(); i++) {
    mpInstanced.setMat4fv(("u_light_space[" + std::to_string(i) + "]").c_str(), light_spaces[i]);
  }
  mpInstanced.setFloat("u_far", CUBE_FAR);
  first = LOCATIONS::TEXTURES::SHADOWMAPS0;
  for (unsigned int i = 0; i < NUM_DIR_LIGHTS; i++) {
    mpInstanced.setInt(("u_dir_shadow_maps[" + std::to_string(i) + "]").c_str(), i + first);
  }
  first += NUM_DIR_LIGHTS;
  for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++) {
    mpInstanced.setInt(("u_point_shadow_maps[" + std::to_string(i) + "]").c_str(), i + first);
  }

// mpLighting.setFloat("u_time", -M_PI_2);
#ifdef NORMALS_DEBUG
  mpNormals.use();
  mpNormals.setVec4fv("u_color", glm::vec4(1.f, 0.f, 1.f, 1.f));
#endif
#ifdef SHADOW_DEBUG
  mShadowsDebug.setModel(glm::translate(glm::mat4(1.f), glm::vec3(0.f, 1.f, 1.f)));
#endif
}

void Playground::update(const float& time)
{
  const bool animatingPos = mCamera.updatePosition(time);
  const bool animatingDir = mCamera.updateDirection(time);

  tifa.update(time);
  dae.update(time);
  whipper.update(time);
  floor.update(time);

  if (animatingPos || animatingDir) {
    return;
  }
#ifdef FOLLOW_WHIPPER
  const glm::vec3& whipperPos = whipper.position();
  camera().setPosition(whipperPos + glm::vec3(0.f, 1.f, 8.f));
  camera().pointTo(whipperPos + glm::vec3(0.f, 1.f, 0.f));
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

#ifdef SHADOW_DEBUG
  glActiveTexture(GL_TEXTURE0); //+ LOCATIONS::TEXTURE::DEBUF
  glBindTexture(GL_TEXTURE_2D, mShadows.mDepthMap1);
  mpShadowsDebug.use();
  mpShadowsDebug.setInt("depthMap", 0);
  mShadowsDebug.render(mpShadowsDebug);

  glBindTexture(GL_TEXTURE_2D, 0);
#endif
}

void Playground::render(const Camera& camera) const
{
#ifdef NORMALS_DEBUG
  const Shader* p_normals = &mpNormals;
#else
  const Shader* p_normals = nullptr;
#endif
#ifdef POINTLIGHT_DEBUG
  const Shader* p_plain = &mpPlain;
#else
  const Shader* p_plain = nullptr;
#endif
  const Shader* shaders[] = {
    &mpSkeletal, &mpLighting, &mpFloor,         &mpInstanced,
    p_plain,     p_normals,   &mpReflectSkybox, &mpSkybox,
  };

#if (NUM_DIR_LIGHTS > 0) or (NUM_POINT_LIGHTS > 0)
  mShadows.bindTextures();
#endif

  renderScene(camera.projection(), camera.view(), camera.position(), camera.front(), shaders);

#if (NUM_DIR_LIGHTS > 0) or (NUM_POINT_LIGHTS > 0)
  mShadows.unbindTextures();
#endif
}

void Playground::renderShadowMap(const glm::mat4& projection, const glm::mat4& view) const
{
#ifdef POINTLIGHT_DEBUG
  const Shader* p_plain = &mp_Plain_shadow;
#else
  const Shader* p_plain = nullptr;
#endif

  const Shader* shaders[] = {
    &mp_Skeletal_shadow,
    &mp_Lighting_shadow,
    &mp_Floor_shadow,
    &mp_Instanced_shadow,
    p_plain,
    nullptr,
    &mp_ReflectSkybox_shadow,
    nullptr,
  };

  return renderScene(projection, view, glm::vec3(0), glm::vec3(0), shaders);
}

void Playground::renderCubeMap(
  const std::vector<glm::mat4>& cube_space, const glm::vec3& lightPos, const float& far
) const
{
#ifdef POINTLIGHT_DEBUG
  const Shader* p_plain = &mp_Plain_cshadow;
#else
  const Shader* p_plain = nullptr;
#endif

  const Shader* shaders[] = {
    &mp_Skeletal_cshadow,
    &mp_Lighting_cshadow,
    &mp_Floor_cshadow,
    &mp_Instanced_cshadow,
    p_plain,
    nullptr,
    &mp_ReflectSkybox_cshadow,
    nullptr,
  };

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

  return renderScene(glm::mat4(1.0), glm::mat4(1.0), glm::vec3(0), glm::vec3(0), shaders);
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
  const Shader& p_skeletal = *shaders[0];
  const Shader& p_lighting = *shaders[1];
  const Shader& p_floor = *shaders[2];
  const Shader& p_instanced = *shaders[3];
  const Shader* p_plain = shaders[4];
  const Shader* p_normals = shaders[5];
  const Shader& p_reflect_skybox = *shaders[6];
  const Shader* p_skybox = shaders[7];

  lightingSettings.updatePointLight0Position();
  lightingSettings.updateSpotLight(view_pos, view_dir, !mSpotlightOn);
  proj_x_view_ub.set(proj_x_view);

  p_skeletal.use();
  p_skeletal.setVec3fv("u_view_pos", view_pos);

  // tifa
  tifa.render(p_skeletal);

  // dae
  dae.render(p_skeletal);

  // whipper
  whipper.render(p_skeletal);

  p_lighting.use();
  p_lighting.setVec3fv("u_view_pos", view_pos);

  glm::mat4 model2b = glm::mat4(1.f);
  model2b = glm::translate(model2b, glm::vec3(0.f, 1.75f, 0.f));
  model2b = glm::rotate(model2b, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
  p_lighting.setMat4fv("u_model", model2b);
  p_lighting.setMat3fv("u_trans_inver_model", glm::mat3(glm::transpose(glm::inverse(model2b))));
  simpleModel.draw(p_lighting);

  mirror.render(p_lighting);
  window.render(p_lighting);

  p_instanced.use();
  p_instanced.setVec3fv("u_view_pos", view_pos);

  box.render(p_instanced);
  grass.render(p_instanced);

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
  p_floor.use();
  p_floor.setVec3fv("u_view_pos", view_pos);
  floor.render(p_floor);
}

void Playground::teardown()
{
  mpSkeletal.free();
  mp_Skeletal_shadow.free();
  mp_Skeletal_cshadow.free();
  tifa.free();
  dae.free();
  whipper.free();

  mpLighting.free();
  mp_Lighting_shadow.free();
  mp_Lighting_cshadow.free();
  simpleModel.free();
  grass.free();
  window.free();
  mirror.free();

  mpFloor.free();
  mp_Floor_shadow.free();
  mp_Floor_cshadow.free();
  floor.free();

  mpInstanced.free();
  mp_Instanced_shadow.free();
  mp_Instanced_cshadow.free();
  box.free();

  mpSkybox.free();
  skybox.free();

#ifdef POINTLIGHT_DEBUG
  pointLightDebug.free();
  mpPlain.free();
#endif

#ifdef NORMALS_DEBUG
  mpNormals.free();
#endif

  mpReflectSkybox.free();
  skyboxReflector.free();
  lightingSettings.free();
  proj_x_view_ub.free();
#ifdef SHADOW_DEBUG
  mShadowsDebug.free();
  mpShadowsDebug.free();
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
#endif
}

void Playground::onChar(const char c)
{
  if (c == 'f') {
    toggleSpotLight();
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
