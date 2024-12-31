#ifndef FLOOR
#define FLOOR

#include "../drawable.h"
#include "../mesh.h"
#include "../texture.h"
#include "../color.h"

class Floor : public Drawable
{
public:
  Floor(unsigned int rows = 5, unsigned columns = 5);
  ~Floor() {};

  void update(const float& time) override;
  void draw(const Shader& shader) const override;
  void free() const override;

  const std::vector<glm::vec3>& positions() const { return mPositions; };
  const std::vector<Color>& colors() const { return mColors; };

private:
  void updateColors();
  void setPositions();
  Mesh mTileMesh;
  std::vector<Texture> mTileTextures;
  unsigned int mRows;
  unsigned int mColumns;
  std::vector<Color> mColors;
  std::vector<glm::vec3> mPositions;
  float mPreviousUpdate;
};


#endif
