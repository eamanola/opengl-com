#include "mesh.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
:
M_INDICES_SIZE(indices.size())
{
  setupMesh(vertices, indices);
}

Mesh::~Mesh()
{
}

void Mesh::setupMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  // position
  glVertexAttribPointer(
    0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)
  );
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
    1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal))
  );
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(
    2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords))
  );
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW
  );

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // keep bound
  glBindBuffer(GL_ARRAY_BUFFER, 0); // Saved to VAO with glVertexAttribPointer
  glBindVertexArray(0);
}

void Mesh::draw(Shader &shader, const Texture* textures, const unsigned int texLen)
{
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for(unsigned int i = 0; i < texLen; i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);

    std::string number;
    std::string name;
    TEXTURE_TYPE type = textures[i].type;
    if(type == TEXTURE_TYPE_DIFFUSE)
    {
      number = std::to_string(diffuseNr++);
      name = "texture_diffuse";
    }
    else if (type == TEXTURE_TYPE_SPECULAR)
    {
      number = std::to_string(specularNr++);
      name = "texture_specular";
    }

    shader.setInt(("u_material." + name + number).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, M_INDICES_SIZE, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  if(texLen > 0)
  {
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // TODO
    shader.setInt("u_material.texture_diffuse1", 0);
    shader.setInt("u_material.texture_specular1", 0);
  }
}

void Mesh::draw(Shader &shader, const Texture* texture)
{
  draw(shader, texture, 1);
}

void Mesh::draw(Shader &shader)
{
  draw(shader, nullptr, 0);
}

void Mesh::draw(Shader &shader, const std::vector<Texture> &textures)
{
  draw(shader, &textures[0], textures.size());
}

const unsigned int Mesh::vao() const
{
  return VAO;
}

void Mesh::free()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}
