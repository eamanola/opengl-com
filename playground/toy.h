#ifndef TOY_H
#define TOY_H

#include "mesh.h"
#include "renderable.h"
#include "texture.h"
#include <vector>

class Toy : public Renderable
{
public:
  Toy(/* args */);
  ~Toy() { }
  void update(const float& time) override { }
  void render(const Shader& shader) const override;
  void free() const override;

private:
  const Mesh mMesh;
  const std::vector<Texture> mTextures;
};

#endif
