#include "playground.h"

// #define FOLLOW_WHIPPER

Playground::Playground()
:
mpSkeletal("./skeletal/skeletal.vs", "./shaders/lighting.fs"),
mpLighting("./shaders/lighting.vs", "./shaders/lighting.fs"),
simpleModel("assets/2b-jumps2/scene.gltf"),
mpSkybox("./playground/skybox/cube.vs", "./playground/skybox/cube.fs"),
mpReflectSkybox("./playground/skybox/reflect-skybox.vs", "./playground/skybox/reflect-skybox.fs"),
// mpReflectSkybox("./shaders/reflect-skybox.vs", "./shaders/single-color.fs"),
#ifdef POINTLIGHT_DEBUG
mpPlain("./shaders/plain.vs", "./shaders/single-color.fs"),
#endif
mSpotlightOn(false)
{
  mpSkeletal.use();
  lightingSettings.setup(mpSkeletal);

  mpLighting.use();
  lightingSettings.setup(mpLighting);
}


Playground::~Playground()
{
}

void Playground::setup()
{
  glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 8.f);
  glm::vec3 pointTo = glm::vec3(0.f);

#ifdef FOLLOW_WHIPPER
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

  if(animatingPos || animatingDir)
  {
    return;
  }
#ifdef FOLLOW_WHIPPER
  glm::vec3 cameraPos = whipper.position() + glm::vec3(0.f, 0.f, 8.f);
  camera().setPosition(cameraPos);
  camera().pointTo(whipper.position());
#endif

  mirror.update(*this);
}

void Playground::render()
{
  const glm::mat4 view = camera().view();
  const glm::mat4 projection = camera().projection();
  const glm::mat4 proj_x_view = projection * view;
  const glm::vec3 view_pos = camera().position();
  glm::mat4 identity = glm::mat4(1.f);

  mpSkeletal.use();
  mpSkeletal.setMat4fv("u_proj_x_view", proj_x_view);
  lightingSettings.setViewPosition(mpSkeletal, view_pos);
  lightingSettings.updatePointLight0Position(mpSkeletal);
  lightingSettings.updateSpotLight(mpSkeletal, view_pos, camera().front(), !mSpotlightOn);

  // tifa
  const glm::mat4 tifaTransform = glm::translate(identity, glm::vec3(-1.5f, 0.f, 1.f));
  tifa.draw(mpSkeletal, tifaTransform);

  // dae
  const glm::mat4 daeTransform = glm::translate(identity, glm::vec3(0.f, 0.f, 1.f));
  dae.draw(mpSkeletal, daeTransform);

  // whipper
  glm::mat4 whipperTransform = glm::translate(identity, whipper.position());
  whipperTransform = glm::rotate(
    whipperTransform, glm::radians(whipper.rotation()), glm::vec3(0.f, 1.f, 0.f)
  );
  whipper.draw(mpSkeletal, whipperTransform);

  mpLighting.use();
  mpLighting.setMat4fv("u_proj_x_view", proj_x_view);
  lightingSettings.setViewPosition(mpLighting, view_pos);
  lightingSettings.updatePointLight0Position(mpLighting);
  lightingSettings.updateSpotLight(mpLighting, view_pos, camera().front(), !mSpotlightOn);

  glm::mat4 m2bm = glm::mat4(1.0);
  m2bm = glm::rotate(m2bm, glm::radians(90.f), glm::vec3(1.0f, 0.f, 0.f));
  m2bm = glm::translate(m2bm, glm::vec3(0.f, 1.5f, -0.75f));
  mpLighting.setMat4fv("u_model", m2bm);
  simpleModel.draw(mpLighting);

  box.draw(mpLighting);
  window.draw(mpLighting);
  mirror.draw(mpLighting);
  grass.draw(mpLighting);

  #ifdef POINTLIGHT_DEBUG
  mpPlain.use();
  mpPlain.setMat4fv("u_proj_x_view", proj_x_view);
  for(unsigned int i = 0; i < lightingSettings.NR_POINT_LIGHTS; i++)
  {
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightingSettings.mLights.positions[i]);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    mpPlain.setVec4fv("u_color", lightingSettings.mLights.colors[i]);
    mpPlain.setMat4fv("u_model", lightModel);
    pointLightDebug.draw(mpPlain);
  }
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
  mpPlain.free();
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

float lastFrame = 0.f;
void Playground::handleInput(const GLFWwindow* window)
{
#ifdef FOLLOW_WHIPPER
  Scene scene = (Scene)*this;
  whipper.handleInput(window, scene);
#else
  const float time = glfwGetTime();
  const float deltaTime = time - lastFrame;
  lastFrame = time;

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

float lastX = 400; float lastY = 300;
bool firstMouse = true;
void Playground::onMouse(const GLFWwindow* window, const double x, const double y)
{
  if(firstMouse)
  {
    lastX = x;
    lastY = y;
    firstMouse = false;
  }

  const float xoffset = x - lastX;
  const float yoffset = y - lastY;
  lastX = x;
  lastY = y;

  camera().changeDirection(xoffset, yoffset);
}

void Playground::onScroll(const GLFWwindow* window, const double x, const double y)
{
  camera().zoom((float)y);
}
