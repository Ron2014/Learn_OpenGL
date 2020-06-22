#include "global.h"
#include "model.h"

#include <iostream>
#include <map>
using namespace std;

extern bool drawNormal = false;
extern unsigned int amount = 0;
extern glm::mat4 *modelMatrices = nullptr;

Model **loading_models = nullptr;
int model_count = 0;
int selected_model = -1;

void switchModels() {
}

void cleanModels() {
  for (int i=0; i<model_count; i++) {
    if(loading_models[i]) delete loading_models[i];
    loading_models[i] = nullptr;
  }
  if (modelMatrices) {
    delete[] modelMatrices;
    modelMatrices = nullptr;
  }
  model_count = 0;
  amount = 0;
}

void initModels(int argc, char *argv[]) {
  cleanModels();

  model_count = argc;
  if (!model_count) return;
  loading_models = new Model*[model_count];
  for (int i=0; i<model_count; i++)
    loading_models[i] = new Model(argv[i], flips[argv[i]]);
}

void renderModels() {
  int shaderId = IDX_MODEL;
  if (genShadow) shaderId = IDX_SIMPLE_DEPTH;

  for (int i=0; i<model_count; i++) {
    Texture2D::reset();

    float radius = 5.0f;
    float lampX = sin(glm::radians(20.0f*i)) * radius;
    float lampZ = cos(glm::radians(20.0f*i)) * radius;
    float scaleV = 0.2f;
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-lampX, 0, -lampZ)); // translate it down so it's at the center of the scen
    model = glm::scale(model, glm::vec3(scaleV));	// it's a bit too big for our scene, so scale it down
    shader[shaderId]->setMatrix4("model", model);

    tex_fb[FB_DEPTH_DIRECT]->use();
    loading_models[i]->Draw(shader[shaderId], &model);
  }
}