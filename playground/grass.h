#ifndef GRASS_H
#define GRASS_H

#include <vector>
#include <glm/glm.hpp>
#include "../mesh.h"

class Grass
{
public:
  Grass(/* args */);
  ~Grass();
  void draw(Shader& shader);
  void free();

private:
  std::vector<glm::vec3> mPositions;
  Mesh mMesh;
  Texture mTexture;
};

#endif
