#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include <vector>
#include "mesh.h"
#include <assimp/scene.h>

class Model
{
public:
  Model(const char* path);
  ~Model();

  void draw(Shader &shader);
  void free();

private:
  std::vector<Mesh> meshes;
  std::string directory;
  std::vector<Texture> textures_loaded;

  void loadModel(const std::string path);
  void processNode(aiNode* node, const aiScene* scene);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(
    aiMaterial *material, aiTextureType aiType, TEXTURE_TYPE type
  );
};


#endif
