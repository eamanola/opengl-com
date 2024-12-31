#ifndef GRASS_H
#define GRASS_H

#include "../drawable.h"
#include <vector>
#include <glm/glm.hpp>
#include "../mesh.h"

class Grass : public Drawable
{
public:
  Grass();
  ~Grass() {};

  void update(const float& time) override {};
  void draw(const Shader& shader) const override;
  void free() const override;

private:
  std::vector<glm::vec3> mPositions;
  Mesh mMesh;
  Texture mTexture;
};

#endif
