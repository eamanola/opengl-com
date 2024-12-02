#include "cube.h"

#include <glad/gl.h>
#include <stb_image.h>
#include <iostream>

#include "vertex.h"
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

Cube::Cube(const char* vShader, const char* fShader) : Shape(vShader, fShader)
{
  VAO = 0;
  VBO = 0;
  EBO = 0;
  enterVertices();
}

Vertex vertices[] = {
  { .position = {  0.5f,  0.5f,  0.5f }, .rgba = { 1.f, 0.f, 0.f } },
  { .position = {  0.5f, -0.5f,  0.5f }, .rgba = { 0.f, 1.f, 0.f } },
  { .position = { -0.5f, -0.5f,  0.5f }, .rgba = { 0.f, 0.f, 1.f } },
  { .position = { -0.5f,  0.5f,  0.5f }, .rgba = { 1.f, 1.f, 0.f } },
  { .position = {  0.5f,  0.5f, -0.5f }, .rgba = { 1.f, 0.f, 0.f } },
  { .position = {  0.5f, -0.5f, -0.5f }, .rgba = { 0.f, 1.f, 0.f } },
  { .position = { -0.5f, -0.5f, -0.5f }, .rgba = { 0.f, 0.f, 1.f } },
  { .position = { -0.5f,  0.5f, -0.5f }, .rgba = { 1.f, 1.f, 0.f } }
};

unsigned int indices[] = {
  0, 1, 2, 0, 2, 3, // front
  4, 5, 1, 4, 1, 0, // right
  7, 6, 5, 7, 5, 4, // back
  3, 2, 6, 3, 6, 7, // left
  4, 0, 3, 4, 3, 7, // top
  1, 5, 6, 1, 6, 2  // bottom
};

void Cube::enterVertices()
{
  program->use();
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (float*)&vertices, GL_STATIC_DRAW);

  // position
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)
  );
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, rgba))
  );
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // keep bound
  glBindBuffer(GL_ARRAY_BUFFER, 0); // Saved to VAO with glVertexAttribPointer
  glBindVertexArray(0);
}

void Cube::render()
{
  program->use();
  glBindVertexArray(VAO);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void Cube::free()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  Shape::free();
}