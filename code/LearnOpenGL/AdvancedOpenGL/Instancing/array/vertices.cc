#include <iostream>
#include <vector>

#include "global.h"
#include "camera.h"

#include <GLFW/glfw3.h>
using namespace glm;
using namespace std;

#define BUFF_LEN 1
extern unsigned int VBO[BUFF_LEN]={0}, VAO[BUFF_LEN]={0};
extern bool rolling = true;

const char *texture_data[TEX_COUNT][2] = {
  {"metal.png", "texture0"},
  {"marble.jpg", "texture0"},
  {"grass.png", "texture0"},
};
extern Texture2D *cube_texture[TEX_COUNT] = {nullptr};
extern Cubemaps *tex_skybox = nullptr;

vector< vector<float> > all_vertices = {
  // points
  {
    // 位置          // 颜色
    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,   
     0.05f,  0.05f,  0.0f, 1.0f, 1.0f     
  },
};

glm::vec3 cube_positions[] = {
  glm::vec3( -1.0f, 0.0f, -1.0f), 
  glm::vec3( 2.0f, 0.0f, 0.0f), 
};

glm::vec3 grass_positions[] = {
  glm::vec3(-1.5f,  0.0f, -0.48f),
  glm::vec3( 1.5f,  0.0f,  0.51f),
  glm::vec3( 0.0f,  0.0f,  0.7f),
  glm::vec3(-0.3f,  0.0f, -2.3f),
  glm::vec3( 0.5f,  0.0f, -0.6f),
};

void cleanCubeData() {
  for (int i=0; i<TEX_COUNT; i++) {
    if(cube_texture[i]) delete cube_texture[i];
    cube_texture[i] = nullptr;
  }
  if (tex_skybox){
    delete tex_skybox;
    tex_skybox = nullptr;
  }
  if (VAO[0]) glDeleteVertexArrays(BUFF_LEN, VAO);
  if (VBO[0]) glDeleteBuffers(BUFF_LEN, VBO);
}

void initCubeData() {
  cleanCubeData();

  glGenVertexArrays(BUFF_LEN, VAO);      // 第二个参数实际上表示一个数组, 第一个参数表示数组大小.
  glGenBuffers(BUFF_LEN, VBO);           // 所以如果是一个 unsigned int 得用&, 如 glGenVertexArrays(1, &VBO);

  for (int i=0; i<BUFF_LEN; i++) {
    cout << "Main::genBuffer ------" << VAO[i] << " " << VBO[i] << all_vertices[i].size() << endl;
    glBindVertexArray(VAO[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
    glBufferData(GL_ARRAY_BUFFER, all_vertices[i].size()*sizeof(float), &(all_vertices[i][0]), GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
  }

  glm::vec2 translations[100];
  int index = 0;
  float offset = 0.1f;
  // ID序: 
  // 从下到上
  // 从左到右
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

  unsigned int instanceVBO;
  glGenBuffers(1, &instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(2);
  glVertexAttribDivisor(2, 1);    // 告诉了OpenGL该什么时候更新顶点属性的内容至新一组数据。

  // unbind
  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}

void renderCubes() {
  shader[IDX_POINT]->use();
  shader[IDX_POINT]->setFloat("time", (float)glfwGetTime());
  glBindVertexArray(VAO[IDX_POINT]);
  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
}

void renderPlane() {
}

void renderGrass() {
}

void renderSkybox(Camera::Camera *camera) {
}