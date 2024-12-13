#include "playground.h"

Playground::Playground()
:
camera(),
skeletal("./skeletal/skeletal.glvs", "./shaders/lighting.glfs"),
tifa(),
dae(),
whipper(),
lightingSettings(),
#ifdef POINTLIGHT_DEBUG
pointLightDebug(),
plainProgram("./shaders/plain.glvs", "./shaders/single-color.glfs"),
#endif
m2b(),
lightingProgram("./shaders/lighting.glvs", "./shaders/lighting.glfs"),
mSpotlightOn(false),
box()
{
  skeletal.use();
  lightingSettings.setup(skeletal);

  m2b.loadModel("assets/2b-jumps2/scene.gltf");
  lightingProgram.use();
  lightingSettings.setup(lightingProgram);

  boxPositions.insert(
    boxPositions.end(),
    {
      glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)
    }
  );
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

  tifa.update(time);
  dae.update(time);
  whipper.update(time);
}

void Playground::render()
{
  const glm::mat4 view = camera.view();
  const glm::mat4 projection = camera.projection();
  const glm::mat4 proj_x_view = projection * view;
  const glm::vec3 view_pos = camera.position();

  skeletal.use();
  skeletal.setMat4fv("u_proj_x_view", proj_x_view);
  lightingSettings.setViewPosition(skeletal, view_pos);
  lightingSettings.updatePointLight0Position(skeletal);
  lightingSettings.updateSpotLight(skeletal, view_pos, camera.front(), !mSpotlightOn);
  // tifa
  tifa.draw(skeletal);

  // dae
  dae.draw(skeletal);

  // whipper
  whipper.draw(skeletal);

  lightingProgram.use();
  lightingProgram.setMat4fv("u_proj_x_view", proj_x_view);
  lightingSettings.setViewPosition(lightingProgram, view_pos);
  lightingSettings.updatePointLight0Position(lightingProgram);
  lightingSettings.updateSpotLight(lightingProgram, view_pos, camera.front(), !mSpotlightOn);

  glm::mat4 m2bm = glm::translate(glm::mat4(1.0), glm::vec3(0.f, 1.f, 0.f));
  lightingProgram.setMat4fv("u_model", m2bm);
  m2b.draw(lightingProgram);

  for(unsigned int i = 0; i < boxPositions.size(); i++)
  {
    glm::mat4 boxModel = glm::translate(glm::mat4(1.0), boxPositions[i]);
    boxModel = glm::rotate(boxModel, glm::radians(20.f  * i), glm::vec3(1.0f, 0.3f, 0.5f));
    if(i == boxPositions.size() - 1)
    {
      highlight(box, boxModel);
      lightingProgram.use();
    } else {
      lightingProgram.setMat4fv("u_model", boxModel);
      box.draw(lightingProgram);
    }
  }

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

  m2b.free();
  box.free();
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

float deltaTime = 0.f;
float lastFrame = 0.f;
void Playground::handleInput(GLFWwindow* window)
{
  const float time = glfwGetTime();
  const float deltaTime = time - lastFrame;
  lastFrame = time;

  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.moveForward(2.5f * deltaTime);

  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.moveBackward(2.5f * deltaTime);

  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.moveLeft(2.5f * deltaTime);

  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.moveRight(2.5f * deltaTime);
}

void Playground::onChar(const char c)
{
  if(c == 'f') {
    toggleSpotLight();
  }
}

float lastX = 400; float lastY = 300;
bool firstMouse = true;
void Playground::onMouse(GLFWwindow* window, double x, double y)
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

void Playground::onScroll(GLFWwindow* window, double x, double y)
{
  camera.zoom((float)y);
}

void Playground::toggleSpotLight()
{
  mSpotlightOn = !mSpotlightOn;
}
