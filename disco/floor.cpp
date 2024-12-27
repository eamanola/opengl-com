#include "floor.h"
#include "../shapes.h"
#include "../shader-utils/shader-utils.h"
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
  mColors.resize(mRows * mColumns);

  mPreviousUpdate = 0;

  glm::mat4 model = glm::mat4(1.f);
  model = glm::translate(model, glm::vec3(0.f, -1.f, 1.f));
  model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  setModel(model);

  setPositions();
  updateColors();
}

void Floor::setPositions()
{
  const float offset = 0.005f;
  const float width = 1.f + 2.f * offset;
  const float height = width;
  const float x0 = -(mRows * width) / 2.f + width / 2.f;
  const float y0 = -(mColumns * height) / 2.f + height / 2.f;

  for (unsigned int i = 0; i < mRows; i++)
  {
    for(unsigned int j = 0; j < mColumns; j++)
    {
      mPositions.push_back(glm::vec3(x0 + (i * width), y0 + (j * height), 0.f));
    }
  }
}

void Floor::updateColors()
{
  for(unsigned int i = 0; i < mRows * mColumns; i++)
  {
    if (((float)rand() / (RAND_MAX)) > 0.5f)
    {
      mColors[i] = glm::vec4(
        ((float)rand() / (RAND_MAX)),
        ((float)rand() / (RAND_MAX)),
        ((float)rand() / (RAND_MAX)),
        0.f
      );
    }
  }
}

void Floor::update(const float& time)
{
  if(time - mPreviousUpdate > 0.25f)
  {
    updateColors();
    mPreviousUpdate = time;
  }
}

void Floor::draw(const Shader& shader)
{
  const glm::mat4& model = this->model();

  for (unsigned int i = 0; i < mRows * mColumns; i++)
  {
    shader.setMat4fv("u_model", glm::translate(model, mPositions[i]));
    shader.setVec4fv("u_color", mColors[i]);
    mTileMesh.draw(shader, mTileTextures);
    shader.setVec4fv("u_color", glm::vec4(0.f));
  }
}
