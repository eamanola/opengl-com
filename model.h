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
  virtual void processScene(const aiScene* scene);

private:
  std::string mDirectory;
  std::vector<Texture> mTextures;
  std::vector<std::vector<unsigned int>> mMeshTextureMap;

  Mesh processMesh(const aiScene* scene, const aiMesh* mesh);
  std::vector<unsigned int> loadMaterialTextures(
    const aiScene* scene, const aiMesh* mesh, const aiTextureType aiType, const TEXTURE_TYPE type
  );
};


#endif
