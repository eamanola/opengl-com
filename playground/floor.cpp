#include "floor.h"
#include "../shapes.h"
#include "../shader-utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Floor::Floor(unsigned int rows, unsigned columns)
:
mTileMesh(Shapes::QUAD),
mTileTextures({
  Texture {
    .id = ShaderUtils::loadTexture("./assets/floor-tile.png"),
    .type = TEXTURE_TYPE_DIFFUSE
  },
  Texture {
    .id = ShaderUtils::loadTexture("./assets/floor-tile-spec.png"),
    .type = TEXTURE_TYPE_SPECULAR
  }
}),
mRows(rows),
mColumns(columns)
{
  updateColors();

  mPreviousUpdate = 0;
}

void Floor::updateColors()
{
  mColors.clear();
  for(unsigned int i = 0; i < mRows * mColumns; i++)
  {
    mColors.push_back(glm::vec4(
      ((float)rand() / (RAND_MAX)),
      ((float)rand() / (RAND_MAX)),
      ((float)rand() / (RAND_MAX)),
      0.f
    ));
  }
}

void Floor::update(const float& time)
{
  if(time - mPreviousUpdate > 3)
  {
    updateColors();
    mPreviousUpdate = time;
  }
}

void Floor::draw(Shader& shader)
{
  const float offset = 0.005f;
  const float width = 1.f + 2.f * offset;
  const float height = width;
  const float x0 = -(mRows * width) / 2.f + width / 2.f;
  const float y0 = -(mColumns * height) / 2.f + height / 2.f;

  glm::mat4 model = glm::mat4(1.f);
  model = glm::translate(model, glm::vec3(0.f, -1.f, 1.f));
  model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));

  for (unsigned int i = 0; i < mRows; i++)
  {
    for(unsigned int j = 0; j < mColumns; j++)
    {
      shader.setMat4fv("u_model", glm::translate(model, glm::vec3(x0 + (i * width), y0 + (j * height), 0.f)));
      shader.setVec4fv("u_color", mColors[i * mRows + j]);
      mTileMesh.draw(shader, mTileTextures);
      shader.setVec4fv("u_color", glm::vec4(0.f));
    }
  }
}
