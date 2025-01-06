#include "floor.h"

#include "shaders/attrib-locations.h"
#include "shaders/u-material.h"
#include "shapes.h"
#include "utils/utils.h"
#include <glm/gtc/matrix_transform.hpp>

Floor::Floor(unsigned int rows, unsigned columns) :
  mTileMesh(Shapes::QUAD),
  mTileTexture(Utils::loadTexture2D("./assets/floor-tile.png", TEXTURE_TYPE_DIFFUSE)),
  mRows(rows),
  mColumns(columns)
{
  mColors.resize(mRows * mColumns);

  mPreviousUpdate = 0;

  glm::mat4 model = glm::mat4(1.f);
  model = glm::translate(model, glm::vec3(0.f, -1.f, 1.f));
  model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
  setModel(model);

  updateColors();
  setupOffsets();
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

void Floor::setupOffsets()
{
  glGenBuffers(1, &mOffsetVBO);
  glGenBuffers(1, &mColorsVBO);

  glBindVertexArray(mTileMesh.vao());

  glBindBuffer(GL_ARRAY_BUFFER, mOffsetVBO);
  std::vector<glm::vec3> positions = getOffsets(mRows, mColumns);
  glBufferData(
    GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), &positions[0], GL_STATIC_DRAW
  );
  glVertexAttribPointer(
    ATTRIB_LOCATIONS::FLOOR_OFFSETS, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0
  );
  glEnableVertexAttribArray(ATTRIB_LOCATIONS::FLOOR_OFFSETS);
  glVertexAttribDivisor(ATTRIB_LOCATIONS::FLOOR_OFFSETS, 1);

  glBindBuffer(GL_ARRAY_BUFFER, mColorsVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * mColors.size(), &mColors[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(
    ATTRIB_LOCATIONS::FLOOR_COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Color), (void*)0
  );
  glEnableVertexAttribArray(ATTRIB_LOCATIONS::FLOOR_COLORS);
  glVertexAttribDivisor(ATTRIB_LOCATIONS::FLOOR_COLORS, 1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Floor::updateColors()
{
  for (unsigned int i = 0; i < mRows * mColumns; i++) {
    if (((float)rand() / (RAND_MAX)) > 0.5f) {
      mColors[i] = Color(
        ((float)rand() / (RAND_MAX)),
        ((float)rand() / (RAND_MAX)),
        ((float)rand() / (RAND_MAX)),
        0.1f
      );
    }
  }
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

  Lighting::u_material::bindTextures(shader, &mTileTexture);

  const glm::mat4& model = this->model();
  shader.setMat4fv("u_model", model);
  shader.setMat3fv("u_trans_inver_model", glm::mat3(glm::transpose(glm::inverse(model))));

  glBindBuffer(GL_ARRAY_BUFFER, mColorsVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * mColors.size(), &mColors[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  mTileMesh.drawInstanced(mRows * mColumns);

  Lighting::u_material::unbindTextures(shader, &mTileTexture);

  glDisable(GL_BLEND);
  glEnable(GL_CULL_FACE);
}

void Floor::free() const
{
  Utils::deleteTextures({ mTileTexture });
  glDeleteBuffers(1, &mOffsetVBO);
  glDeleteBuffers(1, &mColorsVBO);
  mTileMesh.free();
}
