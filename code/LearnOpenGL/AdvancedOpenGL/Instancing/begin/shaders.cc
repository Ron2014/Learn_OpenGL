#include "global.h"
#include "model.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

extern Shader *shader[SHADER_NUM] = {nullptr};


void cleanShader() {
  for (int i=0; i<SHADER_NUM; i++) {
    if (shader[i]) delete shader[i];
    shader[i] = nullptr;
  }
}

void initShaders() {
  cleanShader();
  // shader source -> shader object -> shader program
  shader[IDX_POINT] = new Shader("vertex_instanced.shader", "fragment_default.shader");

  glm::vec2 translations[100];
  int index = 0;
  float offset = 0.1f;
  for(int y = -10; y < 10; y += 2){
      for(int x = -10; x < 10; x += 2){
          glm::vec2 translation(0.0f);
          translation.x = (float)x / 10.0f + offset;
          translation.y = (float)y / 10.0f + offset;
          translations[index] = translation;
          shader[IDX_POINT]->setVec2("offsets", translation, index);
          ++index;
      }
  }
  // for (int i=0; i<index; i++) {
  //   cout << translations[i].x << " " << translations[i].y << endl;
  // }
}

void renderPointLights() {
}

void renderCamera(Camera::Camera *camera) {
}