#include "global.h"
#include "model.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

extern Shader *shader[SHADER_NUM] = {nullptr};
extern vec3 directLightDir(2.0f, -4.0f, 1.0f);
extern vec3 pointLightPos(-2.0f, 0.0f, -2.0f);

extern vector<vec3> lightPositions = {
  vec3(0.0f, 0.0f, 49.5f),
  vec3(-1.4f, -1.9f, 9.0f),
  vec3(0.0f, -1.8f, 4.0f),
  vec3(0.8f, -1.7f, 6.0f),
};

extern vector<vec3> lightColors = {
  vec3(200.0f, 200.0f, 200.0f),
  vec3(0.1f, 0.0f, 0.0f),
  vec3(0.0f, 0.0f, 0.2f),
  vec3(0.0f, 0.1f, 0.0f),
};

extern vector<int> OBJ_IDXS = {IDX_CUBE, IDX_PLANE, IDX_MODEL, IDX_TBN_PLANE,
IDX_SIMPLE_DEPTH, IDX_CUBEMAP_DEPTH, };

void cleanShader() {
  for (int i=0; i<SHADER_NUM; i++) {
    if (shader[i]) delete shader[i];
    shader[i] = nullptr;
  }
}

void initShaders() {
  cleanShader();
  // shader source -> shader object -> shader program
  // shader[IDX_CUBE] = new Shader("vertex_cube_shadow.shader", "fragment_shadow.shader", "geometry_gen_normal.shader");  
  shader[IDX_CUBE] = new Shader("vertex_shadow.shader", "fragment_all_shadow.shader");
  shader[IDX_PLANE] = new Shader("vertex_reverse_normal.shader", "fragment_gamma_normal_mapping.shader");
  shader[IDX_MODEL] = new Shader("vertex_model_normal_mapping.shader", "fragment_model_normal_mapping.shader");
  shader[IDX_TBN_PLANE] = new Shader("vertex_parallax.shader", "fragment_gamma_parallax_steep.shader");

  shader[IDX_QUAD] = new Shader("vertex_normal3d.shader", "fragment_debug_depth.shader");
  shader[IDX_DBG_POINT_SHADOW] = new Shader("vertex_debug_cubemap_depth.shader", "fragment_debug_cubemap_depth.shader");     // 调试用
  
  shader[IDX_LAMP] = new Shader("vertex_lighted.shader", "fragment_color.shader");

  shader[IDX_OUTLINE_MODEL] = new Shader("vertex_model_outline.shader", "fragment_outline_color.shader");
  shader[IDX_OUTLINE_MODEL]->setVec4("color", vec4(1.0f, 1.0f, 1.0f, 1.0f));
  
  shader[IDX_SIMPLE_DEPTH] = new Shader("vertex_simple_depth.shader", "fragment_simple_depth.shader");
  shader[IDX_CUBEMAP_DEPTH] = new Shader("vertex_cubemap_depth.shader", "fragment_cubemaps_depth.shader", "geometry_cubemaps_depth.shader");

  shader[IDX_SKYBOX] = new Shader("vertex_skybox.shader", "fragment_skybox.shader");

  shader[IDX_COLOR_BUF_W] = new Shader("vertex_reverse_normal.shader", "fragment_HDR_write.shader");
  shader[IDX_COLOR_BUF_R] = new Shader("vertex_normal3d.shader", "fragment_HDR_read.shader");

  Model::borderShader = shader[IDX_OUTLINE_MODEL];

  // direct light
  vec3 ambientLight(0.1f, 0.1f, 0.1f);
  vec3 diffuseLight(0.2f, 0.2f, 0.2f);
  vec3 specularLight(1.0f, 1.0f, 1.0f);

  for (int i : OBJ_IDXS) {
    shader[i]->setVec3("directLight.ambient", ambientLight);
    shader[i]->setVec3("directLight.diffuse", diffuseLight);
    shader[i]->setVec3("directLight.specular", specularLight);
    shader[i]->setVec3("directLight.direction", directLightDir);
  }

  // spot light
  ambientLight = vec3(0.0f, 0.0f, 0.0f);
  diffuseLight = vec3(0.3f, 0.3f, 0.3f);
  specularLight = vec3(1.0f, 1.0f, 1.0f);

  for (int i: OBJ_IDXS) {
    shader[i]->setVec3("spotLight.ambient", ambientLight);
    shader[i]->setVec3("spotLight.diffuse", diffuseLight);
    shader[i]->setVec3("spotLight.specular", specularLight);

    shader[i]->setFloat("spotLight.constant", 1.0f);
    shader[i]->setFloat("spotLight.linear", 0.022f);
    shader[i]->setFloat("spotLight.quadratic", 0.0019);

    shader[i]->setFloat("spotLight.cutoff", glm::cos(glm::radians(15.0f)));
    shader[i]->setFloat("spotLight.cutoff_outter", glm::cos(glm::radians(20.0f)));
  }
  
  // point light
  for (int j : OBJ_IDXS) {
    shader[j]->setVec3("pointLight.ambient", ambientLight);
    shader[j]->setVec3("pointLight.diffuse", diffuseLight);
    shader[j]->setVec3("pointLight.specular", specularLight);
    
    shader[j]->setFloat("pointLight.constant", 1.0f);
    shader[j]->setFloat("pointLight.linear", 0.09);
    shader[j]->setFloat("pointLight.quadratic", 0.032);
  }

  for (int i: OBJ_IDXS)
    shader[i]->setFloat("material.shininess", 8.0f);

  shader[IDX_PLANE]->setInt("blinn", 1);  
  shader[IDX_PLANE]->setInt("gamma", 0);  

  shader[IDX_TBN_PLANE]->setInt("blinn", 1);  
  shader[IDX_TBN_PLANE]->setInt("gamma", 0);  
}

void renderPointLights() {
  // glBindVertexArray(VAO[IDX_CUBE]);
  // shader[IDX_LAMP]->use();

  // // 点光 洋红
  // glm::mat4 model(1.0f);
  // model = glm::translate(model, pointLightPos);
  // shader[IDX_LAMP]->setMatrix4("model", model);
  // shader[IDX_LAMP]->setVec4("color", vec4(1.0f, 0.0f, 1.0f, 1.0f));
  // glDrawArrays(GL_TRIANGLES, 0, 36);
        
  // // 平行光 青蓝
  // model = glm::mat4(1.0f);
  // model = glm::translate(model, -glm::normalize(directLightDir) * 10.0f);
  // shader[IDX_LAMP]->setMatrix4("model", model);
  // shader[IDX_LAMP]->setVec4("color", glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  // glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(VAO[IDX_CUBE]);
  shader[IDX_LAMP]->use();
  for (int i = 0; i<lightPositions.size(); i++) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, lightPositions[i]);
    shader[IDX_LAMP]->setMatrix4("model", model);
    shader[IDX_LAMP]->setVec4("color", vec4(lightColors[i], 1.0f));
  }
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void renderCamera() {
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera->FieldOfView), float(WIN_WIDTH)/glm::max((float)WIN_HEIGHT,0.01f), NEAR_Z, FAR_Z);
    glm::mat4 view = camera->GetViewMatrix();
    for (int i=0; i<SHADER_NUM; i++) {
      shader[i]->setMatrix4("projection", projection);
      shader[i]->setMatrix4("view", view);
    }

    glm::vec3 lightPos = camera->Position;  // shader的位置
    glm::vec3 lightDir = camera->Front; //glm::normalize(camera->Position + camera->Front * 200.0f - lightPos);
    for (int i : OBJ_IDXS) {
      shader[i]->setVec3("spotLight.position", lightPos);
      shader[i]->setVec3("spotLight.direction", lightDir);
      shader[i]->setVec3("viewPos", camera->Position);
    }
}