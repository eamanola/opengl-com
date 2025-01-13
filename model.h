#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include <assimp/scene.h>
#include <unordered_map>
#include <vector>

class Model
{
public:
  Model() {};
  Model(const char* path) : Model() { loadModel(path); }
  virtual ~Model() { }

  void draw(const Shader& shader) const;
  virtual void free() const;

protected:
  void loadModel(const std::string path);
  virtual void processScene(const aiScene* scene);
  const Mesh* meshes() { return &mMeshes[0]; }

private:
  std::vector<Mesh> mMeshes;
  std::string mDirectory;
  std::vector<std::vector<std::string>> mMeshTextureMap;
  std::unordered_map<std::string, Texture> mTextures;

  Mesh processMesh(const aiScene* scene, const aiMesh* mesh);
  std::vector<std::string>
  loadMaterialTextures(const aiScene* scene, const aiMesh* mesh, const aiTextureType& aiType);
  Texture loadTexture(const aiScene* scene, const aiTextureType& aiType, const char* name);
};

#endif
