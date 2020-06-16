#pragma once

#include <vector>
#include <map>
#include "shader.h"
#include "meshnode.h"
#include "assimp/scene.h"
using namespace std;

#define MODEL_PATH "E:\\GitHub\\Learn_OpenGL\\res\\models\\"

class Model {
    public:
        /*  函数   */
        Model(string path, bool flip=true) {
            if (path.find("\\")==string::npos) {
                char tmp[256];
                sprintf(tmp, "%s%s\\%s.obj", MODEL_PATH, path.c_str(), path.c_str());
                path = tmp;
            }
            loadModel(path, flip);
        }
        ~Model() {
            cout << "~Model:" << directory.substr(directory.find_last_of("\\")) << endl;
            for (auto it : textures_loaded)
                delete it.second;
            for (auto mesh : meshes)
                mesh.Clean();
        }
        void Draw(Shader *shader);   
    private:
        /*  模型数据  */
        vector<Mesh> meshes;
        map<string, Texture2D *> textures_loaded;
        string directory;
        /*  函数   */
        void loadModel(string path, bool flip);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture2D *> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             string typeName);
};