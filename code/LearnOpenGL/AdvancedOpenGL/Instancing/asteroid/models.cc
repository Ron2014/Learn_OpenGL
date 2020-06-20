#include "global.h"
#include "model.h"
#include <map>

extern bool drawNormal = false;
extern unsigned int amount = 0;

Model **loading_models = nullptr;
glm::mat4 *modelMatrices = nullptr;
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

  model_count = argc-1;
  if (!model_count) return;
  amount = atoi(argv[model_count]);

  loading_models = new Model*[model_count];
  for (int i=0; i<model_count; i++)
    loading_models[i] = new Model(argv[i], flips[argv[i]]);

  modelMatrices = new glm::mat4[amount];
  srand(glfwGetTime()); // 初始化随机种子

  float radius = 50.0;
  float offset = 2.5f;
  for(unsigned int i = 0; i < amount; i++) {
      glm::mat4 model(1.0f);
      // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
      float angle = (float)i / (float)amount * 360.0f;
      float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
      
      float x = sin(glm::radians(angle)) * radius + displacement;
      displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
      float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
      displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
      float z = cos(glm::radians(angle)) * radius + displacement;
      model = glm::translate(model, glm::vec3(x, y, z));

      // 2. 缩放：在 0.05 和 0.25f 之间缩放
      float scaleX = (rand() % 20) * 0.01f + 0.05;
      float scaleY = (rand() % 20) * 0.01f + 0.05;
      float scaleZ = (rand() % 20) * 0.01f + 0.05;
      model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));

      // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
      float rotAngle = (rand() % 360);
      // 0.00 -> 1.00
      scaleX = (rand() % 101) * 0.01f;
      scaleY = (rand() % 101) * 0.01f;
      scaleZ = (rand() % 101) * 0.01f;
      model = glm::rotate(model, glm::radians(rotAngle), glm::vec3(scaleX, scaleY, scaleZ));

      // 4. 添加到矩阵的数组中
      modelMatrices[i] = model;
  }  
}

void renderModels() {
  for (int i=0; i<model_count; i++) {
    // 绘制行星
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));

    shader[IDX_MODEL]->use();
    shader[IDX_MODEL]->setMatrix4("model", model);
    loading_models[i]->Draw(shader[IDX_MODEL]);
    Texture2D::reset();                  // use 完之后记得重置

    // 绘制小行星
    for (int j=0; j<amount; j++) {
      shader[IDX_MODEL]->use();
      shader[IDX_MODEL]->setMatrix4("model", modelMatrices[j]);
      loading_models[i]->Draw(shader[IDX_MODEL]);

      Texture2D::reset();                  // use 完之后记得重置
    }
  }
}