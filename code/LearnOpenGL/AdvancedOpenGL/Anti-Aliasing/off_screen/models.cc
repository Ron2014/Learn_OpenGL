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
}

void renderModels() {
}