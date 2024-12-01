#include "shape.h"

#include <glad/gl.h>
#include <stb_image.h>
#include <iostream>

#include "vertex.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

Vertex vertices[] = {
  { .position = {  0.5f,  0.5f, 0.5f }, .rgba = { 1.f, 0.f, 0.f }, .texCoords = { 1.0f, 1.0f } },
  { .position = {  0.5f, -0.5f, 0.5f }, .rgba = { 0.f, 1.f, 0.f }, .texCoords = { 1.0f, 0.0f } },
  { .position = { -0.5f, -0.5f, 0.5f }, .rgba = { 0.f, 0.f, 1.f }, .texCoords = { 0.0f, 0.0f } },
  { .position = { -0.5f,  0.5f, 0.5f }, .rgba = { 1.f, 1.f, 0.f }, .texCoords = { 0.0f, 1.0f } },
  { .position = {  0.5f,  0.5f, -0.5f }, .rgba = { 1.f, 0.f, 0.f }, .texCoords = { 0.0f, 1.0f } },
  { .position = {  0.5f, -0.5f, -0.5f }, .rgba = { 0.f, 1.f, 0.f }, .texCoords = { 0.0f, 0.0f } },
  { .position = { -0.5f, -0.5f, -0.5f }, .rgba = { 0.f, 0.f, 1.f }, .texCoords = { 1.0f, 0.0f } },
  { .position = { -0.5f,  0.5f, -0.5f }, .rgba = { 1.f, 1.f, 0.f }, .texCoords = { 1.0f, 1.0f } }
};

unsigned int indices[] = {
  0, 1, 2, 0, 2, 3, // front
  4, 5, 1, 4, 1, 0, // right
  7, 6, 5, 7, 5, 4, // back
  3, 2, 6, 3, 6, 7, // left
  4, 0, 3, 4, 3, 7, // top
  1, 5, 6, 1, 6, 2  // bottom
};

Position cubePositions[] = {
  {  0.0f,  0.0f,  0.0f  },
  {  2.0f,  5.0f, -15.0f },
  { -1.5f, -2.2f, -2.5f  },
  { -3.8f, -2.0f, -12.3f }
};

Shape::Shape()
{
  program = new Shader("shape.glvs", "shape.glfs");

  VAO = 0;
  VBO = 0;
  EBO = 0;

  enterVertices();

  program->use();
  program->setFloat("texRatio", 0.2f);

  wallTexture = createTexture("./assets/container.jpg");
  if(wallTexture)
  {
    program->setInt("wallTexture", 0);
  }
  faceTexture = createTexture("./assets/awesomeface.png", GL_RGBA);
  if(wallTexture)
  {
    program->setInt("faceTexture", 1);
  }
}

Shape::~Shape()
{
}

void Shape::enterVertices()
{
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

  glVertexAttribPointer(
    2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords))
  );
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // keep bound
  glBindBuffer(GL_ARRAY_BUFFER, 0); // Saved to VAO with glVertexAttribPointer
  glBindVertexArray(0);
}

unsigned int Shape::createTexture(const char * path, GLenum format)
{
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannel;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(path, &width, &height, &nrChannel, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
    glDeleteTextures(1, &texture);
    texture = 0;
  }

  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

void Shape::render()
{
  // float time = (float)glfwGetTime();
  program->use();
  glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -3.f));
  glm::mat4 projection = glm::perspective(glm::radians(85.f), 800.f / 600.f, 0.1f, 100.f);

  program->setMat4fv("view", glm::value_ptr(view));
  program->setMat4fv("projection", glm::value_ptr(projection));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, wallTexture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, faceTexture);
  glBindVertexArray(VAO);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  for(unsigned int i = 0; i < 4; i ++) {
    glm::mat4 model = glm::mat4(1.f);

    Position position = cubePositions[i];
    model = glm::translate(model, glm::vec3(position.x, position.y, position.z));

    float angle = 20.0f * i;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.f, 0.3f, 0.5f));

    program->setMat4fv("model", glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }
  glBindVertexArray(0);
}

void Shape::free()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(program->ID);
  glDeleteTextures(1, &wallTexture);
  glDeleteTextures(1, &faceTexture);
  delete program;
}
