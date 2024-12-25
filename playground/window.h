#ifndef WINDOW_H
#define WINDOW_H

#include "../drawable.h"
#include <vector>
#include <glm/glm.hpp>
#include "../mesh.h"

class Window : public Drawable
{
public:
  Window(/* args */);
  ~Window() {};
  void update(const float& time) override {};
  void draw(const Shader& shader) override;
  void free() override;

private:
  Mesh mMesh;
  Texture mTexture;
};

#endif
