#include "model.h"

#include "shaders/u-material.h"
#include "utils/utils.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

void Model::loadModel(const std::string path)
{
  Assimp::Importer importer;
  unsigned int postProcess = aiProcess_Triangulate |
                             // aiProcess_FlipUVs | //puts (0,0) to top left (DirectX stuff)
                             aiProcess_GenSmoothNormals;
  const aiScene* scene = importer.ReadFile(path, postProcess);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cout << "Error loading model" << importer.GetErrorString() << std::endl;
    return;
  }

  mDirectory = path.substr(0, path.find_last_of('/'));

  processScene(scene);
}

void Model::processScene(const aiScene* scene)
{
  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[i];
    mMeshes.push_back(processMesh(scene, mesh));
  }

  if (scene->HasMaterials()) {
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
      std::vector<unsigned int> texIndices;
      aiMesh* mesh = scene->mMeshes[i];

      std::vector<unsigned int> dMaps =
        loadMaterialTextures(scene, mesh, aiTextureType_DIFFUSE, TEXTURE_TYPE_DIFFUSE);
      texIndices.insert(texIndices.end(), dMaps.begin(), dMaps.end());

      std::vector<unsigned int> sMaps =
        loadMaterialTextures(scene, mesh, aiTextureType_SPECULAR, TEXTURE_TYPE_SPECULAR);
      texIndices.insert(texIndices.end(), sMaps.begin(), sMaps.end());

      mMeshTextureMap.push_back(texIndices);
    }
  }
}

Mesh Model::processMesh(const aiScene* scene, const aiMesh* mesh)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex {
      .position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
      .normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
    };
    if (mesh->mTextureCoords[0]) {
      vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  return Mesh(vertices, indices);
}

std::vector<unsigned int> Model::loadMaterialTextures(
  const aiScene* const scene,
  const aiMesh* const mesh,
  const aiTextureType aiType,
  const TEXTURE_TYPE type
)
{
  std::vector<unsigned int> textureMapping;
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

  for (unsigned int i = 0; i < material->GetTextureCount(aiType); i++) {
    aiString str;
    material->GetTexture(aiType, i, &str);

    bool skip = false;
    for (unsigned int j = 0; j < mTextures.size(); j++) {
      if (std::strcmp(mTextures[j].key.data(), str.C_Str()) == 0) {
        textureMapping.push_back(j);
        skip = true;
        break;
      }
    }

    if (skip) {
      continue;
    }
    // std::cout << "loading: " << mDirectory << '/' << str.C_Str() << std::endl;

    unsigned int textureId;
    const aiTexture* embedded = scene->GetEmbeddedTexture(str.C_Str());
    if (embedded != nullptr) {
      // Points to an array of mWidth * mHeight aiTexel's.
      // The format of the texture data is always ARGB8888
      // If mHeight = 0 this is a pointer to a memory buffer of size mWidth containing the
      // compressed texture data.
      unsigned int length = embedded->mWidth;
      if (embedded->mHeight > 0)
        length *= embedded->mHeight;

      textureId = Utils::loadTexture2D((unsigned char*)embedded->pcData, length);
    } else {
      textureId = Utils::loadTexture2D((mDirectory + '/' + str.C_Str()).c_str());
    }

    Texture texture { .id = textureId, .type = type, .key = str.C_Str() };
    mTextures.push_back(texture);
    textureMapping.push_back(mTextures.size() - 1);
  }

  return textureMapping;
}

void Model::draw(const Shader& shader) const
{
  for (unsigned int i = 0; i < mMeshes.size(); i++) {
    const std::vector<unsigned int>& texIndices = mMeshTextureMap[i];
    std::vector<Texture> textures;

    for (unsigned int j : texIndices) {
      textures.push_back(mTextures[j]);
    }

    Lighting::u_material::bindTextures(shader, &textures[0], textures.size());
    mMeshes[i].draw();
    Lighting::u_material::unbindTextures(shader, &textures[0], textures.size());
  }
}

void Model::free() const
{
  for (unsigned int i = 0; i < mMeshes.size(); i++) {
    mMeshes[i].free();
  }

  Utils::deleteTextures(mTextures);
}
