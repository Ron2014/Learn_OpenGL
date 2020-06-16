#include "model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

void Model::Draw(Shader *shader) {
#ifdef __DEBUG_DRAW
  cout << "Model draw ======" << endl;
#endif
  for (Mesh m : meshes) {
    m.Draw(shader);
  }
}

void Model::loadModel(string path, bool flip){
    cout << "loadModel ------" << path.c_str() << endl;
    Assimp::Importer import;
    GLenum aiProcess = aiProcess_Triangulate | aiProcess_CalcTangentSpace;
    if (flip) aiProcess |= aiProcess_FlipUVs;
    const aiScene *scene = import.ReadFile(path, aiProcess);    

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }

    if (path.find_last_of('/')!=string::npos)
      directory = path.substr(0, path.find_last_of('/'));
    else
      directory = path.substr(0, path.find_last_of('\\'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  for(unsigned int i=0; i<node->mNumMeshes; i++) {
    aiMesh *aMesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(aMesh, scene));
  }
  
  for (unsigned int i=0; i<node->mNumChildren; i++)
    processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture2D *> textures;

  // vertex attributes
  for (int i=0; i<mesh->mNumVertices; i++) {
    Vertex vertex;

    vertex.Position.x = mesh->mVertices[i].x;
    vertex.Position.y = mesh->mVertices[i].y;
    vertex.Position.z = mesh->mVertices[i].z;

    vertex.Normal.x = mesh->mNormals[i].x;
    vertex.Normal.y = mesh->mNormals[i].y;
    vertex.Normal.z = mesh->mNormals[i].z;

    if(mesh->mTextureCoords[0]) { // does the mesh contain texture coordinates?
      vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
      vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
    } else {
      vertex.TexCoords.x = 0.0f;
      vertex.TexCoords.y = 0.0f;
    }

    // tangent
    vertex.Tangent.x = mesh->mTangents[i].x;
    vertex.Tangent.y = mesh->mTangents[i].y;
    vertex.Tangent.z = mesh->mTangents[i].z;
    // bitangent
    vertex.Bitangent.x = mesh->mBitangents[i].x;
    vertex.Bitangent.y = mesh->mBitangents[i].y;
    vertex.Bitangent.z = mesh->mBitangents[i].z;

    vertices.push_back(vertex);
  }

  // faces -> indices
  for( unsigned int i=0; i<mesh->mNumFaces; i++) {
    aiFace f = mesh->mFaces[i];
    for (unsigned int j=0; j<f.mNumIndices; j++)
      indices.push_back(f.mIndices[j]);
  }

  // material
  aiMaterial *m = scene->mMaterials[mesh->mMaterialIndex];

  vector<Texture2D *> diffuseMaps = loadMaterialTextures(m, aiTextureType_DIFFUSE, "material.diffuse");
  if (diffuseMaps.size()) cout << "diffuse:" << diffuseMaps.size() << endl;
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

  vector<Texture2D *> specularMaps = loadMaterialTextures(m, aiTextureType_SPECULAR, "material.specular");
  if (specularMaps.size()) cout << "specular:" << specularMaps.size() << endl;
  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

  vector<Texture2D *> normalMaps = loadMaterialTextures(m, aiTextureType_HEIGHT, "material.normal");    // 法线贴图, 用来表现凹凸面
  if (normalMaps.size()) cout << "normal:" << normalMaps.size() << endl;
  textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

  vector<Texture2D *> heightMaps = loadMaterialTextures(m, aiTextureType_AMBIENT, "material.height");   // 高度贴图??? 和环境光有关???
  if (heightMaps.size()) cout << "height:" << heightMaps.size() << endl;
  textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

  return Mesh(vertices, indices, textures);
}

vector<Texture2D *> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
  vector<Texture2D *> textures;
  aiString path;
  for (int i=0; i<mat->GetTextureCount(type); i++) {
    mat->GetTexture(type, i, &path);
    string apath = path.C_Str();

    if (textures_loaded.find(apath)!=textures_loaded.end()) {
      textures.push_back(textures_loaded[apath]);
      continue;
    }

    Texture2D *tex = new Texture2D(directory + '\\' + apath, typeName, type);
    textures.push_back(tex);
    textures_loaded.insert({apath, tex});
  }

  return textures;
}

