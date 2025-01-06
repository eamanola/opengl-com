#ifndef WINDOW_H
#define WINDOW_H

#include "renderable.h"

#include "mesh.h"
#include <glm/glm.hpp>
#include <vector>

class Window : public Renderable
{
public:
  Window();
  ~Window() { }
  void update(const float& time) override { }
  void render(const Shader& shader) const override;
  void free() const override;

private:
  Mesh mMesh;
  Texture mTexture;
};

#endif
