#include <iostream>
#include <vector>
#include <map>

#include "bufferutil.h"
#include "global.h"
#include <GLFW/glfw3.h>
using namespace glm;
using namespace std;

#define BUFF_LEN 4
extern unsigned int VBO[BUFF_LEN]={0}, VAO[BUFF_LEN]={0};
extern unsigned int FRAME_BUFF_ID=0;

const char *texture_data[TEX_COUNT][2] = {
  {"metal.png",   "texture0"},
  {"marble.jpg",  "texture0"},
  {NULL,          "texture0"},
};
extern Texture2D *cube_texture[TEX_COUNT] = {nullptr};

vector< vector<float> > all_vertices = {
  // plane
  {
         5.0f, -0.5f,  5.0f,  2.0f, 2.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 0.0f,				
  },
  // cube
  {
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
  },
  // mirror
  {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f,
  },
  // quad
  {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
  },
};

vector< vector<unsigned int > > all_attribs = {
  {3, 2,},
  {3, 2,},
  {3, 2,},
  {2, 2,},
};

glm::vec3 cube_positions[] = {
  glm::vec3( -1.0f, 0.0f, -1.0f), 
  glm::vec3(  2.0f, 0.0f,  0.0f), 
};

glm::vec3 monitor_positions[] = {
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
  if (VAO[0]) glDeleteVertexArrays(BUFF_LEN, VAO);
  memset(VAO, 0, sizeof(VAO[0])*BUFF_LEN);
  if (VBO[0]) glDeleteBuffers(BUFF_LEN, VBO);
  memset(VBO, 0, sizeof(VBO[0])*BUFF_LEN);
}

void initCubeData() {
  cleanCubeData();

  ///////////////////////////////////////////////////////////////////////////// 默认帧缓冲
  for (int i=0; i<BUFF_LEN; i++) {
    initBuffer(VAO[i], VBO[i], all_vertices[i].size(), &(all_vertices[i][0]), all_attribs[i]);
  }
  
  // 生成纹理
  for (int i=0; i<2; i++) {
    cube_texture[i] = new Texture2D(texture_data[i][0], texture_data[i][1]);
  }

  ///////////////////////////////////////////////////////////////////////////// 自定义帧缓冲
  cube_texture[TEX_QUAD] = initFrameBuffer(FRAME_BUFF_ID, texture_data[TEX_QUAD][1]);
}

void renderCubes() {
  Shader *s = shader[IDX_CUBE];
  unsigned int v = VAO[IDX_CUBE];
  Texture2D *tex = cube_texture[TEX_CUBE];

  glBindVertexArray(v);

  // for (int i=0; i<(sizeof(cube_positions)/sizeof(glm::vec3)); i++) {
  for (glm::vec3 pos : cube_positions) {
    s->use();  
    tex->use();
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, tex->ID);
    s->setInt(tex->uniform_name, 0);

    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    // model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 1.0f, 0.0f));
    s->setMatrix4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    Texture2D::reset();
  }
}

void renderPlane() {
  Shader *s = shader[IDX_PLANE];
  unsigned int v = VAO[IDX_PLANE];
  Texture2D *tex = cube_texture[TEX_PLANE];

  glBindVertexArray(v);

  s->use();
  tex->use();
  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_2D, tex->ID);
  s->setInt(tex->uniform_name, 0);

  glm::mat4 model(1.0f);
  s->setMatrix4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  Texture2D::reset();
}