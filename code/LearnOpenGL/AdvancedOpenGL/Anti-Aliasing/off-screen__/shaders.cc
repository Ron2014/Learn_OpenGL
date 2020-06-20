#include "global.h"
#include "model.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

extern Shader *shader[SHADER_NUM] = {nullptr};

// point light
glm::vec3 gPointLightPositions[] = {
  glm::vec3( 0.7f,  0.2f,  2.0f),
  glm::vec3( 2.3f, -3.3f, -4.0f),
  glm::vec3(-4.0f,  2.0f, -12.0f),
  glm::vec3( 0.0f,  0.0f, -3.0f),
};

void cleanShader() {
  for (int i=0; i<SHADER_NUM; i++) {
    if (shader[i]) delete shader[i];
    shader[i] = nullptr;
  }
}

void initShaders() {
  cleanShader();
  // shader source -> shader object -> shader program
  shader[IDX_LAMP] = new Shader("vertex_lighted.shader", "fragment_lamp.shader");
}

void renderPointLights() {
  shader[IDX_LAMP]->use();
  glBindVertexArray(VAO[IDX_LAMP]);

  for (int i=0; i<(sizeof(gPointLightPositions)/sizeof(glm::vec3)); i++) {
    glm::vec3 pos = gPointLightPositions[i];
    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    // model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 0.5f, 1.0f));
    model = glm::scale(model, glm::vec3(0.2f));
    shader[IDX_LAMP]->setMatrix4("model", glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
}

void renderCamera(Camera::Camera *camera) {
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera->FieldOfView), float(WIN_WIDTH)/glm::max((float)WIN_HEIGHT,0.01f), NEAR_Z, FAR_Z);
    glm::mat4 view = camera->GetViewMatrix();
    for (int i=0; i<SHADER_NUM; i++) {
      shader[i]->setMatrix4("projection", glm::value_ptr(projection));
      shader[i]->setMatrix4("view", glm::value_ptr(view));
    }
}