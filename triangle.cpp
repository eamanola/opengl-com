#include "triangle.h"

#include <glad/gl.h>
#include <iostream>

#include "rgba-vertex.h"
#include "shader.h"

RGBAVertex vertices[] = {
  { .position = {  0.5f,  0.5f, 0.f }, .rgba = { 1.f, 0.f, 0.f } },
  { .position = { -0.5f,  0.5f, 0.f }, .rgba = { 0.f, 1.f, 0.f } },
  { .position = {  0.0f, -0.5f, 0.f }, .rgba = { 0.f, 0.f, 1.f } },
};

unsigned int indices[] = {
  0, 1, 2
};

Triangle::Triangle()
{
  program = new Shader("triangle.glvs", "triangle.glfs");
  program->use();
  program->setFloat("xOffset", 0.5f);

  compile();
}

Triangle::~Triangle()
{
}

bool Triangle::compile()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (float*)&vertices, GL_STATIC_DRAW);

  // position
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, sizeof(RGBAVertex), (void*)offsetof(RGBAVertex, position)
  );
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1, 4, GL_FLOAT, GL_FALSE, sizeof(RGBAVertex), (void*)(offsetof(RGBAVertex, rgba))
  );
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // keep bound
  glBindBuffer(GL_ARRAY_BUFFER, 0); // Saved to VAO with glVertexAttribPointer
  glBindVertexArray(0);

  return true;
}

void Triangle::render()
{
  program->use();
  glBindVertexArray(VAO);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void Triangle::free()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(program->ID);
  delete program;
}
