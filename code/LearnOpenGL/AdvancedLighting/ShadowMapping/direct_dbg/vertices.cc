#include <iostream>
#include <vector>

#include "bufferutil.h"
#include "global.h"
#include "camera.h"

#include <GLFW/glfw3.h>
using namespace glm;
using namespace std;

#define BUFF_LEN 3
extern unsigned int VBO[BUFF_LEN]={0}, VAO[BUFF_LEN]={0};
extern unsigned int FRAME_BUFFER[FB_NUM]={0};
extern Texture2D *tex_fb[FB_NUM] = {nullptr};
extern bool rolling = true;
extern Texture2D *tex_depth_point[POINT_LIGHT_NUM] = {nullptr};

const char *texture_data[TEX_COUNT][2] = {
  {"container2.png",          "material.diffuse"},
  {"container2_specular.png", "material.specular"},
  {"matrix.jpg",              "material.emission"},
  {"wood.png",                "ourTexture"},
};
extern Texture2D *cube_texture[TEX_COUNT*2] = {nullptr};
extern Cubemaps *tex_skybox = nullptr;

vector< vector<float> > all_vertices = {
  // cube
  {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
    
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
  },
  // plane
  {
        // positions            // normals         // texcoords
         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
         10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f,
  },
  // quad
  {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
  },
};

vector< vector<unsigned int> > all_attrib = {
  {3, 3, 2,},
  {3, 3, 2,},
  {3, 2,},
};

glm::vec3 cube_positions[] = {
    // glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    // glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    // glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f),
};

glm::vec3 grass_positions[] = {
  glm::vec3(-1.5f,  0.0f, -0.48f),
  glm::vec3( 1.5f,  0.0f,  0.51f),
  glm::vec3( 0.0f,  0.0f,  0.7f),
  glm::vec3(-0.3f,  0.0f, -2.3f),
  glm::vec3( 0.5f,  0.0f, -0.6f),
};

void cleanCubeData() {
  for (int i=0; i<TEX_COUNT*2; i++) {
    if(cube_texture[i]) delete cube_texture[i];
    cube_texture[i] = nullptr;
  }
  if (tex_skybox){
    delete tex_skybox;
    tex_skybox = nullptr;
  }
  for (int i=0; i<POINT_LIGHT_NUM; i++) {
    if (tex_depth_point[i]) delete tex_depth_point[i];
    tex_depth_point[i] = nullptr;
  }
  for (int i=0; i<FB_NUM; i++) {
    if(tex_fb[i]) delete tex_fb[i];
    tex_fb[i] = nullptr;
  }
  if (VAO[0]) glDeleteVertexArrays(BUFF_LEN, VAO);
  if (VBO[0]) glDeleteBuffers(BUFF_LEN, VBO);
}

void initCubeData() {
  cleanCubeData();

  for (int i=0; i<BUFF_LEN; i++) {
    initBuffer(VAO[i], VBO[i], all_vertices[i].size(), &(all_vertices[i][0]), all_attrib[i]);
  }

  for (int i=0; i<TEX_COUNT; i++) {
    cube_texture[i] = new Texture2D(texture_data[i][0], texture_data[i][1]);
    cube_texture[i+TEX_COUNT] = new TextureGamma(texture_data[i][0], texture_data[i][1]);
    shader[IDX_CUBE]->setInt(cube_texture[i]->uniform_name, i);
  }

  // for (int i=0; i<POINT_LIGHT_NUM; i++) {
  //   tex_depth_point[i] = initFrameBufferDepth(FRAME_BUFFER_DEPTH, "texture0");
  // }
  tex_fb[FB_DEPTH_DIRECT] = initFrameBufferDepth(FRAME_BUFFER[FB_DEPTH_DIRECT], "texture0");
  // tex_fb[FB_QUAD] = initFrameBuffer(FRAME_BUFFER[FB_QUAD], "texture0");
}

void renderCubes() {
  glBindVertexArray(VAO[IDX_CUBE]);
  shader[IDX_CUBE]->use();
  
  for (int i=0; i<TEX_COUNT-1; i++) {
    if (gamma) cube_texture[i+TEX_COUNT]->use();
    else  cube_texture[i]->use();
  }

  for (int i=0; i<(sizeof(cube_positions)/sizeof(glm::vec3)); i++) {
    glm::vec3 pos = cube_positions[i];
    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    if (rolling) model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 1.0f, 0.0f));
    shader[IDX_CUBE]->setMatrix4("model", model);
    shader[IDX_SIMPLE_DEPTH]->setMatrix4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  
  Texture2D::reset();
}

void renderPlane() {
  glBindVertexArray(VAO[IDX_PLANE]);

  shader[IDX_PLANE]->use();
  if (gamma) cube_texture[TEX_PLANE+TEX_COUNT]->use();
  else  cube_texture[TEX_PLANE]->use();
  shader[IDX_PLANE]->setInt(cube_texture[TEX_PLANE]->uniform_name, 0);

  glm::mat4 model(1.0f);
  // model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
  shader[IDX_PLANE]->setMatrix4("model", model);
  shader[IDX_SIMPLE_DEPTH]->setMatrix4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  Texture2D::reset();
}

void renderGrass() {
}

void renderSkybox() {
}