#ifndef FLOOR
#define FLOOR

#include "renderable.h"

#include "array.h"
#include "color.h"
#include "mesh.h"
#include "texture.h"

class Floor : public Renderable
{
public:
  Floor(unsigned int rows = 5, unsigned columns = 5);
  ~Floor() { }

  void update(const float& time) override;
  void render(const Shader& shader) const override;
  void free() const override;

private:
  void updateColors();
  std::vector<glm::vec3> getOffsets(unsigned int rows, unsigned int cols) const;
  void setupBuffers();
  Mesh mTileMesh;
  Texture mTileTexture;
  const unsigned int mRows;
  const unsigned int mColumns;
  std::vector<Color> mColors;
  float mPreviousUpdate;
  unsigned int mColorsVBO;
};

#endif
