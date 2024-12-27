#include "disco.h"

Disco::Disco()
:
mpSkeletal("./skeletal/skeletal.vs", "./shaders/lighting.fs",
  {
    "#define IN_NR_POINT_LIGHTS 29",
    "#define IN_NR_SPOT_LIGHTS 2",
    "#define IN_NR_DIR_LIGHTS 1"
  }
),
mpLighting("./shaders/lighting.vs", "./shaders/lighting.fs",
  {
    "#define IN_NR_POINT_LIGHTS 29",
    "#define IN_NR_SPOT_LIGHTS 2",
    "#define IN_NR_DIR_LIGHTS 1"
  }
),
mpSkybox("./playground/skybox/cube.vs", "./playground/skybox/cube.fs")
{
  mLastFrame = 0.f;
  mLastX = 400;
  mLastY = 300;
  mFirstMouse = true;
}

Disco::~Disco()
{
}

void Disco::setup()
{
  glm::mat4 modelDae = glm::mat4(1.f);
  modelDae = glm::translate(modelDae, glm::vec3(0.f, -2.5f, 0.f));
  modelDae = glm::rotate(modelDae, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  modelDae = glm::scale(modelDae, glm::vec3(0.25f));
  dae.setModel(modelDae);

  glm::mat4 modelTifa = glm::mat4(1.f);
  modelTifa = glm::translate(modelTifa, glm::vec3(-1.5f, -2.5f, 0.f));
  tifa.setModel(modelTifa);

  glm::mat4 modelWhipper = glm::mat4(1.f);
  modelWhipper = glm::translate(modelWhipper, glm::vec3(1.5f, -2.5f, 0.f));
  modelWhipper = glm::rotate(modelWhipper, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  modelWhipper = glm::scale(modelWhipper, glm::vec3(0.15f));
  whipper.setModel(modelWhipper);

  glm::mat4 modelMirror = glm::mat4(1.f);
  modelMirror = glm::translate(modelMirror, glm::vec3(0.f, -800.f / 600.f / 2.f, -2.5f));
  // modelMirror = glm::scale(modelMirror, glm::vec3(5.f));
  mirror.setModel(modelMirror);

  glm::mat4 floorModel = glm::mat4(1.f);
  floorModel = glm::translate(floorModel, glm::vec3(0.f, -2.5f, 0.f));
  floorModel = glm::rotate(floorModel, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  floor.setModel(floorModel);

  mpSkeletal.use();
  lightingSettings.setup(mpSkeletal);
  lightingSettings.initFloorLights(mpSkeletal, floor);
  lightingSettings.toggelPointLights1to4(mpSkeletal, true);

  mpLighting.use();
  lightingSettings.setup(mpLighting);
  lightingSettings.initFloorLights(mpLighting, floor);

  glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 8.f);
  glm::vec3 pointTo = glm::vec3(0.f);

  const bool animate = false;
  camera().setPosition(cameraPos, animate);
  camera().pointTo(pointTo, animate);
}

void Disco::update(const float &time)
{
  lightingSettings.mLights.positions[0].x = 1.0f + sin(time) * 2.0f;
  lightingSettings.mLights.positions[0].y = sin(time / 2.0f) * 1.0f;

  lightingSettings.mSpotLights.directions[0] =
  glm::normalize(
    glm::vec3(
      sin(time) * 2.f,
      -2.5,
      cos(time + 2.f) * 2.f
    )
    - lightingSettings.mSpotLights.positions[0]
  );
  lightingSettings.mSpotLights.directions[1] =
  glm::normalize(
    glm::vec3(
      cos(time + 2.f) * 2.f,
      -1.5,
      sin(time * 1.5f) * 2.f
    )
    - lightingSettings.mSpotLights.positions[1]
  );

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

  mirror.screenshot(*this, glm::vec3(0.f, 0.f,-2.f));
}

void Disco::render()
{
  const glm::mat4 view = camera().view();
  const glm::mat4 projection = camera().projection();
  const glm::mat4 proj_x_view = projection * view;
  const glm::vec3 view_pos = camera().position();

  mpSkeletal.use();
  mpSkeletal.setMat4fv("u_proj_x_view", proj_x_view);
  lightingSettings.setViewPosition(mpSkeletal, view_pos);
  lightingSettings.updatePointLight0Position(mpSkeletal);
  lightingSettings.updateFloorLights(mpSkeletal, floor);
  lightingSettings.updateSpotLights(mpSkeletal);

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
  lightingSettings.updateFloorLights(mpLighting, floor);
  lightingSettings.updateSpotLights(mpLighting);

  floor.draw(mpLighting);
  // looks good
  // mpLighting.setVec4fv("u_color", floor.colors()[0]);
  mirror.draw(mpLighting);

  // #ifdef DISCO_DEBUG
  // // mpPlain.use();
  // // mpPlain.setMat4fv("u_proj_x_view", proj_x_view);
  // for(unsigned int i = 0; i < lightingSettings.NR_POINT_LIGHTS; i++)
  // {
  //   glm::mat4 model = glm::translate(glm::mat4(1.0), lightingSettings.mLights.positions[i]);
  //   model = glm::scale(model, glm::vec3(0.2f));

  //   mpLighting.setVec4fv("u_color", lightingSettings.mLights.colors[i]);
  //   mpLighting.setMat4fv("u_model", model);
  //   pointLightDebug.draw(mpLighting);
  // }
  // mpLighting.setVec4fv("u_color", glm::vec4(0));

  // const std::vector<glm::vec3> positions = floor.positions();
  // const std::vector<glm::vec4> colors = floor.colors();
  // for(unsigned int i = 0; i < positions.size(); i++)
  // {
  //   glm::mat4 model = glm::translate(floor.model(), positions[i] + glm::vec3(0.f, 0.f, 0.5f));
  //   model = glm::scale(model, glm::vec3(0.2f));
  //   mpLighting.setVec4fv("u_color", colors[i]);
  //   mpLighting.setMat4fv("u_model", model);
  //   pointLightDebug.draw(mpLighting);
  // }
  // mpLighting.setVec4fv("u_color", glm::vec4(0));

  // for(unsigned int i = 0; i < lightingSettings.mSpotLights.positions.size(); i++)
  // {
  //   glm::mat4 model = glm::translate(glm::mat4(1.0), lightingSettings.mSpotLights.positions[i]);
  //   model = glm::scale(model, glm::vec3(0.2f));
  //   mpLighting.setVec4fv("u_color", lightingSettings.mSpotLights.colors[i]);
  //   mpLighting.setMat4fv("u_model", model);
  //   pointLightDebug.draw(mpLighting);

  //   model = glm::translate(glm::mat4(1.0), lightingSettings.mSpotLights.directions[i]);
  //   model = glm::scale(model, glm::vec3(0.2f));
  //   mpLighting.setVec4fv("u_color", lightingSettings.mSpotLights.colors[i]);
  //   mpLighting.setMat4fv("u_model", model);
  //   pointLightDebug.draw(mpLighting);
  // }
  // mpLighting.setVec4fv("u_color", glm::vec4(0));
  // #endif

  // mpSkybox.use();
  // const glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
  // mpSkybox.setMat4fv("u_proj_x_view", projection * skyboxView);
  // skybox.draw(mpSkybox);
}

void Disco::teardown()
{
  tifa.free();
  dae.free();
  whipper.free();
  mpSkeletal.free();

  #ifdef DISCO_DEBUG
  pointLightDebug.free();
  // mpPlain.free();
  #endif

  mpLighting.free();
  mirror.free();

  mpSkybox.free();
  skybox.free();
}

#ifdef DISCO_DEBUG
void Disco::handleInput(const GLFWwindow* window)
{
  const float time = glfwGetTime();
  const float deltaTime = time - mLastFrame;
  mLastFrame = time;

  if(glfwGetKey((GLFWwindow*)window, GLFW_KEY_W) == GLFW_PRESS) camera().moveForward(2.5f * deltaTime);
  else if(glfwGetKey((GLFWwindow*)window, GLFW_KEY_S) == GLFW_PRESS) camera().moveBackward(2.5f * deltaTime);
  else if(glfwGetKey((GLFWwindow*)window, GLFW_KEY_A) == GLFW_PRESS) camera().moveLeft(2.5f * deltaTime);
  else if(glfwGetKey((GLFWwindow*)window, GLFW_KEY_D) == GLFW_PRESS) camera().moveRight(2.5f * deltaTime);
}

void Disco::onMouse(const GLFWwindow* window, const double x, const double y)
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

void Disco::onScroll(const GLFWwindow* window, const double x, const double y)
{
  camera().zoom((float)y);
}
#endif
