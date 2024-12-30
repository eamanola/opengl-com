#include "playground.h"

// #define FOLLOW_WHIPPER

Playground::Playground()
:
mpSkeletal("./skeletal/skeletal.vs", "./shaders/lighting.fs",
  {
    "#define IN_NR_SPOT_LIGHTS 1\n",
    "#define IN_NR_POINT_LIGHTS 4\n",
    "#define IN_NR_DIR_LIGHTS 1\n"
  },
  { "shaders/lighted-shader-defines" }
),
mpLighting("./shaders/lighting.vs", "./shaders/lighting.fs",
  {
    "#define IN_NR_SPOT_LIGHTS 1\n",
    "#define IN_NR_POINT_LIGHTS 4\n",
    "#define IN_NR_DIR_LIGHTS 1\n"
  },
  { "shaders/lighted-shader-defines" }
),
simpleModel("assets/2b-jumps2/scene.gltf"),
mpSkybox("./playground/skybox/cube.vs", "./playground/skybox/cube.fs"),
mpReflectSkybox("./playground/skybox/reflect-skybox.vs", "./playground/skybox/reflect-skybox.fs"),
// #ifdef POINTLIGHT_DEBUG
// no noticible difference in changing, use mpLighting for now
// mpPlain("./shaders/lighting.vs", "./shaders/lighting.fs"),
// #endif
mSpotlightOn(false)
{
  mpSkeletal.use();
  lightingSettings.setup(mpSkeletal);

  mpLighting.use();
  lightingSettings.setup(mpLighting);

  mLastFrame = 0.f;
  mLastX = 400;
  mLastY = 300;
  mFirstMouse = true;
}

Playground::~Playground()
{
}

void Playground::setup()
{
  glm::mat4 model2b = glm::mat4(1.f);
  model2b = glm::rotate(model2b, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
  model2b = glm::translate(model2b, glm::vec3(0.f, 1.5f, -0.75f));
  simpleModel.setModel(model2b);

  glm::mat4 modelDae = glm::mat4(1.f);
  modelDae = glm::translate(modelDae, glm::vec3(0.f, -1.f, 1.f));
  modelDae = glm::rotate(modelDae, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  modelDae = glm::scale(modelDae, glm::vec3(0.25f));
  dae.setModel(modelDae);

  glm::mat4 modelTifa = glm::mat4(1.f);
  modelTifa = glm::translate(modelTifa, glm::vec3(0.f, -1.f, 1.f));
  modelTifa = glm::translate(modelTifa, glm::vec3(-1.5f, 0.f, 0.f));
  tifa.setModel(modelTifa);

  glm::mat4 modelWhipper = glm::mat4(1.f);
  modelWhipper = glm::translate(modelWhipper, glm::vec3(0.f, -1.f, 1.f));
  modelWhipper = glm::translate(modelWhipper, glm::vec3(1.5f, 0.f, 0.f));
  modelWhipper = glm::rotate(modelWhipper, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  modelWhipper = glm::scale(modelWhipper, glm::vec3(0.15f));
  whipper.setModel(modelWhipper);

  glm::mat4 mirrorModel = glm::mat4(1.0);
  mirrorModel = glm::translate(mirrorModel, glm::vec3(2.5f, 0.5f, 0.f));
  mirrorModel = glm::rotate(mirrorModel, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
  mirrorModel = glm::scale(mirrorModel, glm::vec3(0.3f));
  mirror.setModel(mirrorModel);

  glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 8.f);
  glm::vec3 pointTo = glm::vec3(0.f);

#ifdef FOLLOW_WHIPPER
  // glm::vec3 whipperPos = glm::vec3(whipper.model()[3]);
  cameraPos = whipper.position() + cameraPos;
  pointTo = whipper.position();
#endif

  const bool animate = false;
  camera().setPosition(cameraPos, animate);
  camera().pointTo(pointTo, animate);
}

void Playground::update(const float &time)
{
  lightingSettings.mLights.positions[0].x = 1.0f + sin(time) * 2.0f;
  lightingSettings.mLights.positions[0].y = sin(time / 2.0f) * 1.0f;

  const bool animatingPos = camera().updatePosition(time);
  const bool animatingDir = camera().updateDirection(time);

  tifa.update(time);
  dae.update(time);
  whipper.update(time);
  floor.update(time);

  if(animatingPos || animatingDir)
  {
    return;
  }
#ifdef FOLLOW_WHIPPER
  glm::vec3 cameraPos = whipper.position() + glm::vec3(0.f, 0.f, 8.f);
  camera().setPosition(cameraPos);
  camera().pointTo(whipper.position());
#endif

  mirror.screenshot(*this);
}

void Playground::render()
{
  const glm::mat4 view = camera().view();
  const glm::mat4 projection = camera().projection();
  const glm::mat4 proj_x_view = projection * view;
  const glm::vec3 view_pos = camera().position();

  mpSkeletal.use();
  mpSkeletal.setMat4fv("u_proj_x_view", proj_x_view);
  lightingSettings.setViewPosition(mpSkeletal, view_pos);
  lightingSettings.updatePointLight0Position(mpSkeletal);
  lightingSettings.updateSpotLight(mpSkeletal, view_pos, camera().front(), !mSpotlightOn);

  // tifa
  tifa.draw(mpSkeletal);

  // dae
  dae.draw(mpSkeletal);

  // whipper
  whipper.draw(mpSkeletal);

  mpLighting.use();
  mpLighting.setMat4fv("u_proj_x_view", proj_x_view);
  lightingSettings.setViewPosition(mpLighting, view_pos);
  lightingSettings.updatePointLight0Position(mpLighting);
  lightingSettings.updateSpotLight(mpLighting, view_pos, camera().front(), !mSpotlightOn);

  simpleModel.draw(mpLighting);
  box.draw(mpLighting);
  window.draw(mpLighting);
  mirror.draw(mpLighting);
  grass.draw(mpLighting);
  floor.draw(mpLighting);

  #ifdef POINTLIGHT_DEBUG
  // mpPlain.use();
  // mpPlain.setMat4fv("u_proj_x_view", proj_x_view);
  for(unsigned int i = 0; i < lightingSettings.NR_POINT_LIGHTS; i++)
  {
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightingSettings.mLights.positions[i]);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    mpLighting.setVec4fv("u_color", lightingSettings.mLights.colors[i]);
    mpLighting.setMat4fv("u_model", lightModel);
    pointLightDebug.draw(mpLighting);
  }
  mpLighting.setVec4fv("u_color", glm::vec4(0));
  #endif

  mpSkybox.use();
  const glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
  mpSkybox.setMat4fv("u_proj_x_view", projection * skyboxView);
  skybox.draw(mpSkybox);

  mpReflectSkybox.use();
  mpReflectSkybox.setMat4fv("u_proj_x_view", proj_x_view);
  mpReflectSkybox.setVec3fv("u_view_pos", view_pos);
  skyboxReflector.draw(mpReflectSkybox);
}

void Playground::teardown()
{
  tifa.free();
  dae.free();
  whipper.free();
  mpSkeletal.free();

  #ifdef POINTLIGHT_DEBUG
  pointLightDebug.free();
  // mpPlain.free();
  #endif

  simpleModel.free();
  box.free();
  grass.free();
  window.free();
  mpLighting.free();
  mirror.free();

  mpSkybox.free();
  skybox.free();

  mpReflectSkybox.free();
  skyboxReflector.free();
}

void Playground::highlight(Box &box, glm::mat4 model)
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
  Scene scene = (Scene)*this;
  whipper.handleInput(window, scene);
#else
  const float time = glfwGetTime();
  const float deltaTime = time - mLastFrame;
  mLastFrame = time;

  if(glfwGetKey((GLFWwindow*)window, GLFW_KEY_W) == GLFW_PRESS) camera().moveForward(2.5f * deltaTime);
  else if(glfwGetKey((GLFWwindow*)window, GLFW_KEY_S) == GLFW_PRESS) camera().moveBackward(2.5f * deltaTime);
  else if(glfwGetKey((GLFWwindow*)window, GLFW_KEY_A) == GLFW_PRESS) camera().moveLeft(2.5f * deltaTime);
  else if(glfwGetKey((GLFWwindow*)window, GLFW_KEY_D) == GLFW_PRESS) camera().moveRight(2.5f * deltaTime);
#endif
}

void Playground::onChar(const char c)
{
  if(c == 'f') {
    toggleSpotLight();
  }
  else if(c == ' ')
  {
    std::cout << " ";
  }
}

void Playground::onMouse(const GLFWwindow* window, const double x, const double y)
{
  if(mFirstMouse)
  {
    mLastX = x;
    mLastY = y;
    mFirstMouse = false;
  }

  const float xoffset = x - mLastX;
  const float yoffset = y - mLastY;
  mLastX = x;
  mLastY = y;

  camera().changeDirection(xoffset, yoffset);
}

void Playground::onScroll(const GLFWwindow* window, const double x, const double y)
{
  camera().zoom((float)y);
}
