#include "model.h"

#include "shaders/u-material.h"
#include "utils/utils.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

void Model::loadModel(const std::string path)
{
  Assimp::Importer importer;
  unsigned int postProcess =
    aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace;
  //  aiProcess_FlipUVs | // puts (0,0) to top left (DirectX stuff)

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
      std::vector<std::string> textures;
      aiMesh* mesh = scene->mMeshes[i];

      std::vector<std::string> dMaps = loadMaterialTextures(scene, mesh, aiTextureType_DIFFUSE);
      textures.insert(textures.end(), dMaps.begin(), dMaps.end());

      std::vector<std::string> sMaps = loadMaterialTextures(scene, mesh, aiTextureType_SPECULAR);
      textures.insert(textures.end(), sMaps.begin(), sMaps.end());

      std::vector<std::string> hMaps = loadMaterialTextures(scene, mesh, aiTextureType_HEIGHT);
      textures.insert(textures.end(), hMaps.begin(), hMaps.end());

      std::vector<std::string> nMaps = loadMaterialTextures(scene, mesh, aiTextureType_NORMALS);
      textures.insert(textures.end(), nMaps.begin(), nMaps.end());

      std::vector<std::string> eMaps = loadMaterialTextures(scene, mesh, aiTextureType_EMISSIVE);
      textures.insert(textures.end(), eMaps.begin(), eMaps.end());

      mMeshTextureMap.push_back(textures);
    }
  }
}

Mesh Model::processMesh(const aiScene* scene, const aiMesh* mesh)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex {
      .position = Position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
      .normal = Normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
      .tangent = Tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z),
    };
    if (mesh->mTextureCoords[0]) {
      vertex.texCoords = TexCoords(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
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

std::vector<std::string> Model::loadMaterialTextures(
  const aiScene* const scene, const aiMesh* const mesh, const aiTextureType& aiType
)
{
  std::vector<std::string> textureMapping;
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

  for (unsigned int i = 0; i < material->GetTextureCount(aiType); i++) {
    aiString path;
    material->GetTexture(aiType, i, &path);

    const char* cPath = path.C_Str();

    if (mTextures.find(cPath) == mTextures.end()) {
      mTextures[cPath] = loadTexture(scene, aiType, cPath);
    }

    textureMapping.push_back(cPath);
  }

  return textureMapping;
}

Texture Model::loadTexture(const aiScene* scene, const aiTextureType& aiType, const char* path)
{
  // std::cout << "loading: " << mDirectory << '/' << str.C_Str() << std::endl;

  TEXTURE_TYPE type;
  switch (aiType) {
  case aiTextureType_DIFFUSE:
    type = TEXTURE_TYPE_DIFFUSE;
    break;
  case aiTextureType_SPECULAR:
    type = TEXTURE_TYPE_SPECULAR;
    break;
  case aiTextureType_NORMALS:
    type = TEXTURE_TYPE_NORMALS;
    break;
  case aiTextureType_HEIGHT:
    type = TEXTURE_TYPE_HEIGHT;
    break;
  case aiTextureType_EMISSIVE:
    type = TEXTURE_TYPE_EMISSIVE;
    break;

  default:
    assert(false);
    break;
  }

  const aiTexture* embedded = scene->GetEmbeddedTexture(path);
  if (embedded != nullptr) {
    // Points to an array of mWidth * mHeight aiTexel's.
    // The format of the texture data is always ARGB8888
    // If mHeight = 0 this is a pointer to a memory buffer of size mWidth containing the
    // compressed texture data.
    unsigned int length = embedded->mWidth;
    if (embedded->mHeight > 0)
      length *= embedded->mHeight;

    return Utils::Textures::loadTexture2D((unsigned char*)embedded->pcData, length, type);
  }

  return Utils::Textures::loadTexture2D((mDirectory + '/' + path).c_str(), type);
}

void Model::draw(const Shader& shader) const
{
  for (unsigned int i = 0; i < mMeshes.size(); i++) {
    std::vector<Texture> textures;

    for (std::string path : mMeshTextureMap[i]) {
      textures.push_back(mTextures.at(path));
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

  std::vector<Texture> textures;
  for (auto t : mTextures) {
    textures.push_back(t.second);
  }

  Utils::Textures::deleteTextures(textures);
}
