#ifndef FLOOR
#define FLOOR

#include "../drawable.h"
#include "../mesh.h"
#include "../texture.h"

class Floor : public Drawable
{
public:
  Floor(unsigned int rows = 5, unsigned columns = 5);
  ~Floor() {};

  void update(const float& time) override;
  void draw(const Shader& shader) override;
  void free() override {
    mTileMesh.free();
    for(Texture t : mTileTextures) glDeleteTextures(1, &t.id);
  }

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
