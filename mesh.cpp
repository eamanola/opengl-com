#include "mesh.h"
#include "shaders/attrib-locations.h"

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

  glVertexAttribPointer(
    ATTRIB_LOCATIONS::POSITION,
    3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)
  );
  glEnableVertexAttribArray(ATTRIB_LOCATIONS::POSITION);

  bool enableNormals = false;
  for(Vertex vertex : vertices)
  {
    if(vertex.normal != glm::vec3(0))
    {
      enableNormals = true;
      break;
    }
  }

  if(enableNormals)
  {
    glVertexAttribPointer(
      ATTRIB_LOCATIONS::NORMAL,
      3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal))
    );
    glEnableVertexAttribArray(ATTRIB_LOCATIONS::NORMAL);
  }

  bool enableTexCoords = false;
  for(Vertex vertex : vertices)
  {
    if(vertex.texCoords != glm::vec2(0))
    {
      enableTexCoords = true;
      break;
    }
  }

  if(enableTexCoords)
  {
    glVertexAttribPointer(
      ATTRIB_LOCATIONS::TEX_COORDS,
      2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords))
    );
    glEnableVertexAttribArray(ATTRIB_LOCATIONS::TEX_COORDS);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW
  );

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // keep bound
  glBindBuffer(GL_ARRAY_BUFFER, 0); // Saved to VAO with glVertexAttribPointer
  glBindVertexArray(0);
}

void Mesh::draw(const Shader &shader, const Texture* textures, const unsigned int texLen)
{
  assert(texLen <= 2);
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
    for(unsigned int i = 0; i < texLen; i ++)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    glActiveTexture(0);

    // TODO
    shader.setInt("u_material.texture_diffuse1", 0);
    shader.setInt("u_material.texture_specular1", 0);
  }
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
