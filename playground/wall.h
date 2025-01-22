#ifndef WALL_H
#define WALL_H

#include "mesh.h"
#include "renderable.h"
#include "texture.h"
#include <vector>

class Wall : public Renderable
{
public:
  Wall(/* args */);
  ~Wall() { }
  void update(const float& time) override { }
  void render(const Shader& shader) const override;
  void free() const override;

private:
  const Mesh mMesh;
  const std::vector<Texture> mTextures;
};

#endif
