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

  const Array<glm::vec3> positions() const { return Array(mPositions); }
  const Array<Color> colors() const { return Array(mColors); }

private:
  void updateColors();
  void setPositions();
  Mesh mTileMesh;
  Texture mTileTexture;
  const unsigned int mRows;
  const unsigned int mColumns;
  std::vector<Color> mColors;
  std::vector<glm::vec3> mPositions;
  float mPreviousUpdate;
};

#endif
