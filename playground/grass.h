#ifndef GRASS_H
#define GRASS_H

#include "renderable.h"

#include "mesh.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <vector>

class Grass : public Renderable
{
public:
  Grass();
  ~Grass() { }

  void update(const float& time) override { }
  void render(const Shader& shader) const override;
  void free() const override;

private:
  std::vector<glm::mat4> models() const;
  void setupBuffers();
  std::vector<glm::vec3> mPositions;
  Mesh mMesh;
  Texture mTexture;
  unsigned int mModelsVBO;
};

#endif
