#ifndef MODEL_H
#define MODEL_H

#include "drawable.h"
#include "shader.h"
#include <vector>
#include "mesh.h"
#include <assimp/scene.h>

class Model
{
public:
  Model() {};
  Model(const char* path) : Model() { loadModel(path); }
  virtual ~Model() {};

  void draw(const Shader &shader);
  virtual void free();

protected:
  void loadModel(const std::string path);
  virtual void processScene(const aiScene* scene);
  const std::vector<Mesh>& meshes() { return mMeshes; };

private:
  std::vector<Mesh> mMeshes;
  std::string mDirectory;
  std::vector<Texture> mTextures;
  std::vector<std::vector<unsigned int>> mMeshTextureMap;

  Mesh processMesh(const aiScene* scene, const aiMesh* mesh);
  std::vector<unsigned int> loadMaterialTextures(
    const aiScene* scene, const aiMesh* mesh, const aiTextureType aiType, const TEXTURE_TYPE type
  );
};


#endif
