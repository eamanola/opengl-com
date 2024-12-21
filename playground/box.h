#ifndef BOX_H
#define BOX_H

#include "../mesh.h"

class Box
{
public:
  Box(/* args */);
  ~Box();
  void draw(Shader &shader);
  void free();

private:
  Mesh mMesh;
  std::vector<Texture> mTextures;
  std::vector<glm::vec3> mPositions;
};


#endif
