#include "global.h"
#include "model.h"
#include <map>

Model **loading_models = nullptr;
int model_count = 0;
int selected_model = -1;

// 不知为何 backpack的贴图没有Y方向颠倒, 这省去了flip操作.
map<string, bool> flips = { {"nanosuit", true}, {"backpack", false} };

void switchModels() {
  if (selected_model>=0) loading_models[selected_model]->ShowBorder(false);
  if (model_count) {
    selected_model = (selected_model+1)%model_count;
    cout << "select " << selected_model << endl;
    loading_models[selected_model]->ShowBorder(true);
  }
}

void cleanModels() {
  for (int i=0; i<model_count; i++) {
    if(loading_models[i]) delete loading_models[i];
    loading_models[i] = nullptr;
  }
}

void initModels(int n, char *path[]) {
  cleanModels();

  model_count = n;
  if (!model_count) return;

  loading_models = new Model*[model_count];
  for (int i=0; i<model_count; i++)
    loading_models[i] = new Model(path[i], flips[path[i]]);
}

void renderModels() {
  for (int i=0; i<model_count; i++) {
    shader[IDX_MODEL]->use();
    tex_skybox->use();

    float radius = 10.0f;
    float lampX = sin(glm::radians(10.0f*i)) * radius;
    float lampZ = cos(glm::radians(10.0f*i)) * radius;
    float scaleV = 0.2f;
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-lampX, 0, -lampZ)); // translate it down so it's at the center of the scen
    model = glm::scale(model, glm::vec3(scaleV));	// it's a bit too big for our scene, so scale it down
    shader[IDX_MODEL]->setMatrix4("model", glm::value_ptr(model));
    loading_models[i]->Draw(shader[IDX_MODEL], &model);
    Texture2D::reset();                  // use 完之后记得重置
    Cubemaps::reset();
  }
}