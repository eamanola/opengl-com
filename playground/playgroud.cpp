#include "playground.h"

// #define FOLLOW_WHIPPER

Playground::Playground()
:
skeletal("./skeletal/skeletal.glvs", "./shaders/lighting.glfs"),
#ifdef POINTLIGHT_DEBUG
pointLightDebug(),
plainProgram("./shaders/plain.glvs", "./shaders/single-color.glfs"),
#endif
lightingProgram("./shaders/lighting.glvs", "./shaders/lighting.glfs"),
mSpotlightOn(false)
{
  skeletal.use();
  lightingSettings.setup(skeletal);

  simpleModel.loadModel("assets/2b-jumps2/scene.gltf");

  lightingProgram.use();
  lightingSettings.setup(lightingProgram);

#ifdef FOLLOW_WHIPPER
  glm::vec3 cameraPos = whipper.position() + glm::vec3(0.f, 0.f, 8.f);
  glm::vec3 cameraDir = glm::normalize(whipper.position() - cameraPos);
#else
  glm::vec3 cameraPos = glm::vec3(0.f) + glm::vec3(0.f, 0.f, 8.f);
  glm::vec3 cameraDir = glm::normalize(glm::vec3(0.f) - cameraPos);
#endif
  camera.setPosition(cameraPos, true);
  camera.setDirection(cameraDir, true);
}


Playground::~Playground()
{
}

void Playground::setup()
{

}

void Playground::update(const float &time)
{
  lightingSettings.mLights.positions[0].x = 1.0f + sin(time) * 2.0f;
  lightingSettings.mLights.positions[0].y = sin(time / 2.0f) * 1.0f;

  camera.updatePosition(time);
  camera.updateDirection(time);

  tifa.update(time);
  dae.update(time);
  whipper.update(time);

#ifdef FOLLOW_WHIPPER
  if(time > 2.f)
  {
    glm::vec3 cameraPos = whipper.position() + glm::vec3(0.f, 0.f, 8.f);
    camera.setPosition(cameraPos);

    glm::vec3 cameraDir = glm::normalize(whipper.position() - camera.position());
    camera.setDirection(cameraDir);
  }
#endif
}

void Playground::render()
{
  const glm::mat4 view = camera.view();
  const glm::mat4 projection = camera.projection();
  const glm::mat4 proj_x_view = projection * view;
  const glm::vec3 view_pos = camera.position();
  glm::mat4 identity = glm::mat4(1.f);

  skeletal.use();
  skeletal.setMat4fv("u_proj_x_view", proj_x_view);
  lightingSettings.setViewPosition(skeletal, view_pos);
  lightingSettings.updatePointLight0Position(skeletal);
  lightingSettings.updateSpotLight(skeletal, view_pos, camera.front(), !mSpotlightOn);

  lightingProgram.use();
  lightingProgram.setMat4fv("u_proj_x_view", proj_x_view);
  lightingSettings.setViewPosition(lightingProgram, view_pos);
  lightingSettings.updatePointLight0Position(lightingProgram);
  lightingSettings.updateSpotLight(lightingProgram, view_pos, camera.front(), !mSpotlightOn);

  skeletal.use();
  // tifa
  const glm::mat4 tifaTransform = glm::translate(identity, glm::vec3(-1.5f, 0.f, 1.f));
  tifa.draw(skeletal, tifaTransform);

  // dae
  const glm::mat4 daeTransform = glm::translate(identity, glm::vec3(0.f, 0.f, 1.f));
  dae.draw(skeletal, daeTransform);

  // whipper
  glm::mat4 whipperTransform = glm::translate(identity, whipper.position());
  whipperTransform = glm::rotate(
    whipperTransform, glm::radians(whipper.rotation()), glm::vec3(0.f, 1.f, 0.f)
  );
  whipper.draw(skeletal, whipperTransform);

  lightingProgram.use();
  grass.draw(lightingProgram);

  glm::mat4 m2bm = glm::mat4(1.0);
  m2bm = glm::rotate(m2bm, glm::radians(90.f), glm::vec3(1.0f, 0.f, 0.f));
  m2bm = glm::translate(m2bm, glm::vec3(0.f, 1.5f, -0.75f));
  lightingProgram.setMat4fv("u_model", m2bm);
  simpleModel.draw(lightingProgram);

  box.draw(lightingProgram);
  window.draw(lightingProgram);

  #ifdef POINTLIGHT_DEBUG
  plainProgram.use();
  plainProgram.setMat4fv("u_proj_x_view", proj_x_view);
  for(unsigned int i = 0; i < lightingSettings.NR_POINT_LIGHTS; i++)
  {
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightingSettings.mLights.positions[i]);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    plainProgram.setVec3fv("color", lightingSettings.mLights.colors[i]);
    plainProgram.setMat4fv("u_model", lightModel);
    pointLightDebug.draw(plainProgram);
  }
  #endif
}

void Playground::teardown()
{
  tifa.free();
  dae.free();
  whipper.free();
  skeletal.free();

  #ifdef POINTLIGHT_DEBUG
  pointLightDebug.free();
  plainProgram.free();
  #endif

  simpleModel.free();
  box.free();
  grass.free();
  window.free();
  lightingProgram.free();
}

void Playground::highlight(Box &box, glm::mat4 model)
{
  glEnable(GL_STENCIL_TEST);
  glClear(GL_STENCIL_BUFFER_BIT);

  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  glStencilMask(0xFF);

  lightingProgram.use();
  lightingProgram.setMat4fv("u_model", model);
  box.draw(lightingProgram);

  glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
  glStencilMask(0x00);
  // glDisable(GL_DEPTH_TEST); ?
  plainProgram.use();
  plainProgram.setVec3fv("color", glm::vec3(1.0, 1.0, 1.0));
  plainProgram.setMat4fv("u_model", glm::scale(model, glm::vec3(1.1)));
  box.draw(plainProgram);

  glStencilMask(0xFF);
  glStencilFunc(GL_ALWAYS, 1, 0xFF);
  // glEnable(GL_DEPTH_TEST);

  glDisable(GL_STENCIL_TEST);
}

float lastFrame = 0.f;
void Playground::handleInput(const GLFWwindow* window)
{
#ifdef FOLLOW_WHIPPER
  whipper.handleInput(window, camera);
#else
  const float time = glfwGetTime();
  const float deltaTime = time - lastFrame;
  lastFrame = time;

  if(glfwGetKey((GLFWwindow*)window, GLFW_KEY_W) == GLFW_PRESS) camera.moveForward(2.5f * deltaTime);
  else if(glfwGetKey((GLFWwindow*)window, GLFW_KEY_S) == GLFW_PRESS) camera.moveBackward(2.5f * deltaTime);
  else if(glfwGetKey((GLFWwindow*)window, GLFW_KEY_A) == GLFW_PRESS) camera.moveLeft(2.5f * deltaTime);
  else if(glfwGetKey((GLFWwindow*)window, GLFW_KEY_D) == GLFW_PRESS) camera.moveRight(2.5f * deltaTime);
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

  camera.changeDirection(xoffset, yoffset);
}

void Playground::onScroll(const GLFWwindow* window, const double x, const double y)
{
  camera.zoom((float)y);
}

void Playground::toggleSpotLight()
{
  mSpotlightOn = !mSpotlightOn;
}
