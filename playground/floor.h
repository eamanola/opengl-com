#ifndef FLOOR
#define FLOOR

#include "../mesh.h"
#include "../texture.h"

class Floor
{
public:
  Floor(unsigned int rows = 5, unsigned columns = 5);
  ~Floor() {};

  void free() {
    mTileMesh.free();
    for(Texture t : mTileTextures) glDeleteTextures(1, &t.id);
  }
  void update(const float& time);
  void draw(Shader& shader);

private:
  void updateColors();
  Mesh mTileMesh;
  std::vector<Texture> mTileTextures;
  unsigned int mRows;
  unsigned int mColumns;
  std::vector<glm::vec4> mColors;
  float mPreviousUpdate;
};


#endif
