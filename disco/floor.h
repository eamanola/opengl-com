#ifndef FLOOR
#define FLOOR

#include "drawable.h"
#include "mesh.h"
#include "texture.h"
#include "color.h"
#include "array.h"

class Floor : public Drawable
{
public:
  Floor(unsigned int rows = 5, unsigned columns = 5);
  ~Floor() {};

  void update(const float& time) override;
  void draw(const Shader& shader) const override;
  void free() const override;

  const Array<glm::vec3> positions() const { return Array(mPositions); };
  const Array<Color> colors() const { return Array(mColors); };

private:
  void updateColors();
  void setPositions();
  Mesh mTileMesh;
  Texture mTileTextures;
  const unsigned int mRows;
  const unsigned int mColumns;
  std::vector<Color> mColors;
  std::vector<glm::vec3> mPositions;
  float mPreviousUpdate;
};


#endif
