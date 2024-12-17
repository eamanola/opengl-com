#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include <vector>
#include "mesh.h"
#include <assimp/scene.h>

class Model
{
public:
  Model();
  virtual ~Model();

  void loadModel(const std::string path);
  void draw(Shader &shader);
  virtual void free();

protected:
  std::vector<Mesh> meshes;
  virtual void processScene(const aiScene* const scene);

private:
  std::string mDirectory;
  std::vector<Texture> mTextures;
  std::vector<std::vector<unsigned int>> mMeshTextureMap;

  Mesh processMesh(const aiMesh* const mesh, const aiScene* const scene);
  std::vector<unsigned int> loadMaterialTextures(
    const aiScene* const scene, const aiMesh* const mesh,
    const aiTextureType aiType, const TEXTURE_TYPE type
  );
};


#endif
