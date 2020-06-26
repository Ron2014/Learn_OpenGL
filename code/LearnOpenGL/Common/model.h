#pragma once

#include <vector>
#include <map>
#include "shader.h"
#include "meshnode.h"
#include "assimp/scene.h"
using namespace std;

#if FUTURE_WINDOWS
#define MODEL_PATH "E:/GitHub/Learn_OpenGL/res/models/"
#elif FUTURE_OSX
#define MODEL_PATH "/Users/ron/Documents/GitHub/Learn_OpenGL/res/models/"
#else
#define MODEL_PATH "./res/models/"
#endif


extern map<string, bool> flips;
class ModelCore {
    public:
        /*  函数   */
        ModelCore(string path, bool flip=true);
        ~ModelCore();
        void Draw(Shader *shader, glm::mat4 *model=NULL);
        vector<Mesh> &GetMesh();
        void DrawAmount(Shader *shader, int amount);
        void ShowBorder(bool visible=true);
        int ref;
    private:
        /*  模型数据  */
        vector<Mesh> meshes;
        map<string, Texture2D *> textures_loaded;
        string directory;
        glm::vec3 centerPos;
        int mesh_count;
        bool border;

        /*  函数   */
        void loadModel(string path, bool flip);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture2D *> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             string typeName);
};

class Model {
public:
    enum Model_Movement {
        UNKNOWN,
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        RISE,
        FALL,
    };
    /*  函数   */
    Model(string path, bool flip=true);
    ~Model();
    void Draw(Shader *shader, glm::mat4 *model=NULL) {core->Draw(shader, model);}
    vector<Mesh> &GetMesh() { return core->GetMesh(); }
    void DrawAmount(Shader *shader, int amount) {core->DrawAmount(shader, amount);}
    void ShowBorder(bool visible=true) {core->ShowBorder(visible);}
    static map<string, ModelCore *> resources;
    static Shader *borderShader;
    ModelCore *core;
};