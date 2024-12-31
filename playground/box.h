#ifndef BOX_H
#define BOX_H

#include "../drawable.h"
#include "../mesh.h"

class Box : public Drawable
{
public:
  Box();
  ~Box() {};
  virtual void update(const float& time) override {};
  void draw(const Shader &shader) const override;
  void free() const override;

private:
  Mesh mMesh;
  std::vector<Texture> mTextures;
  std::vector<glm::vec3> mPositions;
};


#endif
