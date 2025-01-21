#include "floor.h"

#include "gl-utils/gl-utils.h"
#include "shaders/locations.h"
#include "shaders/u-material.h"
#include "shaders/u-model.h"
#include "shapes.h"
#include "utils/utils.h"
#include <glm/gtc/matrix_transform.hpp>

Floor::Floor(unsigned int rows, unsigned columns) :
  mTileMesh(Shapes::QUAD),
  mTileTexture(Utils::Textures::loadTexture2D("./assets/floor-tile.png", TEXTURE_TYPE_DIFFUSE)),
  mRows(rows),
  mColumns(columns)
{
  mColors.resize(mRows * mColumns);

  mPreviousUpdate = 0;

  updateColors();
  setupBuffers();
}

std::vector<glm::vec3> Floor::getOffsets(unsigned int rows, unsigned int cols) const
{
  std::vector<glm::vec3> positions;
  const float offset = 0.005f;
  const float width = 1.f + 2.f * offset;
  const float height = width;
  const float x0 = -(rows * width) / 2.f + width / 2.f;
  const float y0 = -(cols * height) / 2.f + height / 2.f;

  for (unsigned int i = 0; i < rows; i++) {
    for (unsigned int j = 0; j < cols; j++) {
      positions.push_back(glm::vec3(x0 + (i * width), y0 + (j * height), 0.f));
    }
  }

  return positions;
}

void Floor::updateColors()
{
  const float OPACITY = 0.0f; // 0.4 from floor-tile.png
  for (unsigned int i = 0; i < mRows * mColumns; i++) {
    if (((float)rand() / (RAND_MAX)) > 0.5f) {
      mColors[i] = Color(
        ((float)rand() / (RAND_MAX)),
        ((float)rand() / (RAND_MAX)),
        ((float)rand() / (RAND_MAX)),
        OPACITY
      );
    }
  }
}

void Floor::setupBuffers()
{
  glBindVertexArray(mTileMesh.vao());

  std::vector<glm::vec3> offsets = getOffsets(mRows, mColumns);
  GLUtils::VertexAttribPointer offset = {
    .location = LOCATIONS::ATTRIBS::FLOOR_OFFSETS,
    .size = 3,
    .stride = sizeof(glm::vec3),
    .offset = (void*)0,
    .divisor = 1,
  };

  GLUtils::addVertexBuffer(mOffsetVBO, &offsets[0], sizeof(glm::vec3) * offsets.size(), { offset });

  GLUtils::VertexAttribPointer color = {
    .location = LOCATIONS::ATTRIBS::COLOR,
    .size = 4,
    .stride = sizeof(Color),
    .offset = (void*)0,
    .divisor = 1,
  };

  GLUtils::addVertexBuffer(
    mColorsVBO,
    &mColors[0],
    sizeof(glm::vec3) * mColors.size(),
    { color },
    GLUtils::BufferUsage::DYNAMIC
  );

  glBindVertexArray(0);
}

void Floor::update(const float& time)
{
  if (time - mPreviousUpdate > 0.25f) {
    updateColors();
    mPreviousUpdate = time;
  }
}

void Floor::render(const Shader& shader) const
{
  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  u_model::setUModel(shader, model());

  glBindBuffer(GL_ARRAY_BUFFER, mColorsVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * mColors.size(), &mColors[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  Lighting::u_material::bindTexture(shader, &mTileTexture);

  mTileMesh.drawInstanced(mRows * mColumns);

  Lighting::u_material::unbindTexture(shader, &mTileTexture);

  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);
}

void Floor::free() const
{
  Utils::Textures::deleteTexture(mTileTexture);
  mTileMesh.free();
  const unsigned int buffers[] = { mOffsetVBO, mColorsVBO };
  glDeleteBuffers(2, buffers);
}
