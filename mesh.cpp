#include "mesh.h"

Mesh::Mesh(
  std::vector<Vertex> vertices,
  std::vector<unsigned int> indices,
  std::vector<Texture> textures
) : vertices(vertices), indices(indices), textures(textures)
{
  setupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::setupMesh()
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

void Mesh::draw(Shader &shader)
{
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for(unsigned int i = 0; i < textures.size(); i++)
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
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  if(textures.size())
  {
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

const unsigned int Mesh::vao() const
{
  return VAO;
}

void Mesh::free()
{
  for(unsigned int i = 0; i < textures.size(); i ++)
  {
    glDeleteTextures(1, &i);
  }
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}
