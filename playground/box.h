#ifndef BOX_H
#define BOX_H

#include "renderable.h"

#include "mesh.h"
#include "texture.h"

class Box : public Renderable
{
public:
  Box();
  ~Box() { }

  void update(const float& time) override { }
  void render(const Shader& shader) const override;
  void free() const override;

private:
  Mesh mMesh;
  void setupBuffers();
  const std::vector<glm::mat4> models() const;
  std::vector<Texture> mTextures;
  std::vector<glm::vec3> mPositions;
};

#endif
