#include "global.h"
#include "model.h"

#include <iostream>
#include <map>
using namespace std;

extern bool drawNormal = false;
extern unsigned int amount = 0;
extern glm::mat4 *modelMatrices = nullptr;

extern Model **loading_models = nullptr;
extern int model_count = 0;
extern int selected_model = -1;
extern glm::vec3 *model_pos = nullptr;

void switchModels() {
  if (selected_model>=0)
    loading_models[selected_model]->ShowBorder(false);

  if (model_count) {
    selected_model = (selected_model+1)%(model_count+1);
    if (selected_model==model_count) selected_model = -1;
    cout << "select " << selected_model << endl;
    if(selected_model>=0) loading_models[selected_model]->ShowBorder(true);
  }
}

void cleanModels() {
  for (int i=0; i<model_count; i++) {
    if(loading_models[i]){
      delete loading_models[i];
    }
    loading_models[i] = nullptr;
  }
  if (modelMatrices) {
    delete[] modelMatrices;
    modelMatrices = nullptr;
  }
  if (model_pos) {
    delete[] model_pos;
    model_pos = nullptr;
  }
  model_count = 0;
  amount = 0;
}

void initModels(int argc, char *argv[]) {
  cleanModels();

  model_count = argc;
  if (!model_count) return;
  loading_models = new Model*[model_count];
  model_pos = new glm::vec3[model_count];
  for (int i=0; i<model_count; i++) {
    loading_models[i] = new Model(argv[i], flips[argv[i]]);
    
    float radius = 5.0f;
    float lampX = sin(glm::radians(20.0f*i)) * radius;
    float lampZ = cos(glm::radians(20.0f*i)) * radius;
    model_pos[i] = glm::vec3(-lampX, -0.5f, -lampZ);
  }
}

void moveModel(unsigned int movement, float ratio) {
  if (selected_model<0) return;
  glm::vec3 trans(1.0f);
  switch(movement) {
    case Model::FORWARD:{
      trans = vec3(0.0f, 0.0f, -0.1f) * ratio;
    } break;
    case Model::BACKWARD:{
      trans = vec3(0.0f, 0.0f, 0.1f) * ratio;
    } break;
    case Model::LEFT:{
      trans = vec3(-0.1f, 0.0f, 0.0f) * ratio;
    } break;
    case Model::RIGHT:{
      trans = vec3(0.1f, 0.0f, 0.0f) * ratio;
    } break;
    case Model::RISE:{
      trans = vec3(0.0f, 0.1f, 0.0f) * ratio;
    } break;
    case Model::FALL:{
      trans = vec3(0.0f, -0.1f, 0.0f) * ratio;
    } break;
  }
  glm::mat4 m(1.0f);
  m = glm::translate(m, trans);
  model_pos[selected_model] = glm::vec3(m * glm::vec4(model_pos[selected_model], 1.0f));
}

void renderModels() {
  int shaderId = IDX_MODEL;
  if (genShadow) shaderId = IDX_SIMPLE_DEPTH;

  for (int i=0; i<model_count; i++) {
    Texture2D::reset();

    float scaleV = 0.2f;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, model_pos[i]); // translate it down so it's at the center of the scen
    model = glm::scale(model, glm::vec3(scaleV));	// it's a bit too big for our scene, so scale it down
    shader[shaderId]->setMatrix4("model", model);

    tex_fb[FB_DEPTH_MAP]->use();
    loading_models[i]->Draw(shader[shaderId], &model);
  }
}