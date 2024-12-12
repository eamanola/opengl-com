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
  std::string directory;
  std::vector<Texture> textures_loaded;

  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(
    aiMaterial *material, aiTextureType aiType, TEXTURE_TYPE type
  );
};


#endif
