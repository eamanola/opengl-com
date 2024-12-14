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
  std::string directory;
  std::vector<Texture> textures_loaded;

  Mesh processMesh(const aiMesh* const mesh, const aiScene* const scene);
  std::vector<Texture> loadMaterialTextures(
    const aiScene* const scene, const aiMaterial* const material,
    const aiTextureType aiType, const TEXTURE_TYPE type
  );
};


#endif
