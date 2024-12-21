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

  Shader skeletal;
  Tifa tifa;
  Dae dae;
  Whipper whipper;
  Lighting lightingSettings;
  #ifdef POINTLIGHT_DEBUG
  Light pointLightDebug;
  Shader plainProgram;
  #endif
  Model simpleModel;
  Shader lightingProgram;
  bool mSpotlightOn;
  Box box;
  Grass grass;
  Window window;
};


