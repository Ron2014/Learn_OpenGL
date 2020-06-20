#include <iostream>
#include <vector>

#include "global.h"
#include "camera.h"

#include <GLFW/glfw3.h>
using namespace glm;
using namespace std;

#define BUFF_LEN 1
extern unsigned int VBO[BUFF_LEN]={0}, VAO[BUFF_LEN]={0};

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
    -0.5f,  0.5f, // 左上
     0.5f,  0.5f, // 右上
     0.5f, -0.5f, // 右下
    -0.5f, -0.5f  // 左下
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
  }

  // unbind
  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);
}

void renderCubes() {
  shader[IDX_POINT]->use();
  glBindVertexArray(VAO[IDX_POINT]);
  glDrawArrays(GL_POINTS, 0, 4);
}

void renderPlane() {
}

void renderGrass() {
}

void renderSkybox(Camera::Camera *camera) {
}