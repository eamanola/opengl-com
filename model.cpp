#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader-utils.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::loadModel(const std::string path)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "Error loading model" << importer.GetErrorString() << std::endl;
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));

  processScene(scene);
}

void Model::processScene(const aiScene* scene)
{
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
  for(unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }
  for(unsigned int i = 0; i < node->mNumChildren; i++)
  {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for(unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex vertex {
      .position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
      .normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
    };
    if (mesh->mTextureCoords[0])
    {
      vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    vertices.push_back(vertex);
  }

  for(unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for(unsigned int j = 0; j < face.mNumIndices; j++)
    {
      indices.push_back(face.mIndices[j]);
    }
  }

  if(mesh->mMaterialIndex > 0)
  {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> dMaps = loadMaterialTextures(
      material, aiTextureType_DIFFUSE, TEXTURE_TYPE_DIFFUSE
    );
    textures.insert(textures.end(), dMaps.begin(), dMaps.end());

    std::vector<Texture> sMaps = loadMaterialTextures(
      material, aiTextureType_SPECULAR, TEXTURE_TYPE_SPECULAR
    );
    textures.insert(textures.end(), sMaps.begin(), sMaps.end());
  }

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(
  aiMaterial *material, aiTextureType aiType, TEXTURE_TYPE type
)
{
  std::vector<Texture> textures;
  for(unsigned int i = 0; i < material->GetTextureCount(aiType); i++)
  {
    aiString str;
    material->GetTexture(aiType, i, &str);

    bool skip = false;
    for(unsigned int j = 0; j < textures_loaded.size(); j++)
    {
      if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
      {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }

    if(skip)
    {
      continue;
    }
    std::cout << "loading: " << directory << '/' << str.C_Str() << std::endl;

    Texture texture {
      .id = ShaderUtils::loadTexture(str.C_Str(), directory),
      .type = type,
      .path = str.C_Str()
    };
    textures.push_back(texture);
    textures_loaded.push_back(texture);
  }

  return textures;
}

void Model::draw(Shader &shader)
{
  for(unsigned int i = 0; i < meshes.size(); i++)
  {
    meshes[i].draw(shader);
  }
}

void Model::free()
{
  for(unsigned int i = 0; i < meshes.size(); i++)
  {
    meshes[i].free();
  }
}
