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

  if (argc!=2) return;
  model_count = argc-1;
  amount = atoi(argv[model_count]);

  loading_models = new Model*[model_count];
  for (int i=0; i<model_count; i++)
    loading_models[i] = new Model(argv[i], flips[argv[i]]);

  modelMatrices = new glm::mat4[amount];
  srand(glfwGetTime()); // 初始化随机种子

  float radius = 50.0;
  float offset = 12.5f;
  // float radius = 150.0;
  // float offset = 25.0f;
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
  
  // cout << sizeof(glm::mat4) << " " << sizeof(glm::vec4) << endl;

  GLsizei mat4Size = sizeof(glm::mat4);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[IDX_MODEL]);
  glBufferData(GL_ARRAY_BUFFER, amount * mat4Size, &modelMatrices[0], GL_STATIC_DRAW);

  for (Mesh mesh : loading_models[0]->core->meshes) {
    glBindVertexArray(mesh.VAO);

/*
    // 暂不支持 顶点属性的类型大于vec4    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 16, GL_FLOAT, GL_FALSE, mat4Size, (void*)0);
    glVertexAttribDivisor(3, 1);
*/
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
  }
}

void renderModels() {
  for (int i=0; i<model_count; i++) {
    // 绘制行星
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));

    shader[IDX_PLANET]->use();
    shader[IDX_PLANET]->setMatrix4("model", model);
    loading_models[i]->Draw(shader[IDX_PLANET]);

    // 绘制小行星
    shader[IDX_MODEL]->use();
    loading_models[i]->DrawAmount(shader[IDX_MODEL], amount);
  }
}