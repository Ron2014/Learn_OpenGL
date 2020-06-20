#include "model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Shader *Model::borderShader = nullptr;
map<string, ModelCore *> Model::resources = {};

Model::Model(string path, bool flip) {
  ModelCore *model;
  if (resources.find(path)==resources.end()) {
    resources.insert({path, new ModelCore(path, flip)});
  }
  model = resources[path];
  model->ref++;
  core = model;
}

Model::~Model() {
  core->ref--;
  if (core->ref<=0)
    delete core;
}

ModelCore::ModelCore(string path, bool flip):border(false),centerPos(glm::vec3(0.0f)),mesh_count(0),ref(0)  {
  if (path.find("\\")==string::npos) {
      char tmp[256];
      sprintf(tmp, "%s%s\\%s.obj", MODEL_PATH, path.c_str(), path.c_str());
      path = tmp;
  }
  loadModel(path, flip);
}

ModelCore::~ModelCore() {
#ifdef __DEBUG_LOAD
  cout << "~ModelCore:" << directory.substr(directory.find_last_of("\\")) << endl;
#endif
  for (auto it : textures_loaded)
      delete it.second;
  for (auto mesh : meshes)
      mesh.Clean();
}

void ModelCore::ShowBorder(bool visible) {
  border = visible;
}

void ModelCore::Draw(Shader *shader, glm::mat4 *model) {
#ifdef __DEBUG_DRAW
  cout << "ModelCore draw ======" << endl;
#endif
  if (border && model) {
#ifdef OUTLINE_DEPTH
    glClear(GL_STENCIL_BUFFER_BIT);
#endif
  }

  for (Mesh m : meshes)
    m.Draw(shader);

  if (border && model) {
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);      // 缓冲区取反
#ifndef OUTLINE_DEPTH
    glDisable(GL_DEPTH_TEST);
#endif

    Model::borderShader->use();
    Model::borderShader->setMatrix4("model", glm::value_ptr(*model));
    for (Mesh m : meshes)
      m.Draw(Model::borderShader);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
#ifndef OUTLINE_DEPTH
    glEnable(GL_DEPTH_TEST);
#endif 
  }
}

void ModelCore::loadModel(string path, bool flip){
#ifdef __DEBUG_LOAD
    cout << "loadModel ------" << path.c_str() << endl;
#endif
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
    if (mesh_count) centerPos /= mesh_count;
}

void ModelCore::processNode(aiNode *node, const aiScene *scene) {
  for(unsigned int i=0; i<node->mNumMeshes; i++) {
    aiMesh *aMesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(aMesh, scene));
  }
  
  for (unsigned int i=0; i<node->mNumChildren; i++)
    processNode(node->mChildren[i], scene);
}

Mesh ModelCore::processMesh(aiMesh *mesh, const aiScene *scene) {
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture2D *> textures;
  glm::vec3 center(0.0f);

#ifdef __DEBUG_LOAD
    float mark = 1.0f;
#endif
  // vertex attributes
  for (int i=0; i<mesh->mNumVertices; i++) {
    Vertex vertex;

    vertex.Position.x = mesh->mVertices[i].x;
    vertex.Position.y = mesh->mVertices[i].y;
    vertex.Position.z = mesh->mVertices[i].z;

    center += vertex.Position;

#ifdef __DEBUG_LOAD
    bool show = false;
    if (vertex.Position.x < -mark || vertex.Position.x > mark) {
        show = true;
        mark = glm::abs(vertex.Position.x);
    } else if (vertex.Position.y < -mark || vertex.Position.y > mark) {
        show = true;
        mark = glm::abs(vertex.Position.y);
    } else if (vertex.Position.z < -mark || vertex.Position.x > mark) {
        show = true;
        mark = glm::abs(vertex.Position.z);
    }
    if (show) cout << vertex.Position.x << " " << vertex.Position.y << " " << vertex.Position.z << endl;
#endif

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
  if (mesh->mNumVertices) center /= mesh->mNumVertices;
  centerPos += center;
  mesh_count ++;

  // faces -> indices
  for( unsigned int i=0; i<mesh->mNumFaces; i++) {
    aiFace f = mesh->mFaces[i];
    for (unsigned int j=0; j<f.mNumIndices; j++)
      indices.push_back(f.mIndices[j]);
  }

  // material
  aiMaterial *m = scene->mMaterials[mesh->mMaterialIndex];

  vector<Texture2D *> diffuseMaps = loadMaterialTextures(m, aiTextureType_DIFFUSE, "material.diffuse");
#ifdef __DEBUG_LOAD
  if (diffuseMaps.size()) cout << "diffuse:" << diffuseMaps.size() << endl;
#endif
  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

  vector<Texture2D *> specularMaps = loadMaterialTextures(m, aiTextureType_SPECULAR, "material.specular");
#ifdef __DEBUG_LOAD
  if (specularMaps.size()) cout << "specular:" << specularMaps.size() << endl;
#endif
  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

  vector<Texture2D *> normalMaps = loadMaterialTextures(m, aiTextureType_HEIGHT, "material.normal");    // 法线贴图, 用来表现凹凸面
#ifdef __DEBUG_LOAD
  if (normalMaps.size()) cout << "normal:" << normalMaps.size() << endl;
#endif
  textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

  vector<Texture2D *> heightMaps = loadMaterialTextures(m, aiTextureType_AMBIENT, "material.height");   // 高度贴图??? 和环境光有关???
#ifdef __DEBUG_LOAD
  if (heightMaps.size()) cout << "height:" << heightMaps.size() << endl;
#endif
  textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

  return Mesh(vertices, indices, textures, center);
}

vector<Texture2D *> ModelCore::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
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

