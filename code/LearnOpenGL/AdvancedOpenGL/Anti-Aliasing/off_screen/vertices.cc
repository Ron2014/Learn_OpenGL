#include <iostream>
#include <vector>

#include "bufferutil.h"
#include "global.h"
#include "camera.h"

#include <GLFW/glfw3.h>
using namespace glm;
using namespace std;

#define BUFF_LEN 2
extern unsigned int VBO[BUFF_LEN]={0}, VAO[BUFF_LEN]={0};
extern bool rolling = true;
extern unsigned int FRAME_BUFF_ID_0 = 0;
extern unsigned int FRAME_BUFF_ID_1 = 0;

const char *texture_data[TEX_COUNT][2] = {
  {"metal.png", "texture0"},
};
extern Texture2D *cube_texture[TEX_COUNT] = {nullptr};
extern Cubemaps *tex_skybox = nullptr;

vector< vector<float> > all_vertices = {
  // cube
  {
        // positions       
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
  },
  {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
  },
};

vector< vector<unsigned int > > all_attribs = {
  {3, },
  {2, 2,},
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

void initCubeData(int sample) {
  cleanCubeData();

  ///////////////////////////////////////////////////////////////////////////// 默认帧缓冲
  for (int i=0; i<BUFF_LEN; i++) {
    initBuffer(VAO[i], VBO[i], all_vertices[i].size(), &(all_vertices[i][0]), all_attribs[i]);
  }

  ///////////////////////////////////////////////////////////////////////////// 自定义帧缓冲 0
  cube_texture[TEX_MSAA] = initFrameBufferSample(FRAME_BUFF_ID_0, sample);

  ///////////////////////////////////////////////////////////////////////////// 自定义帧缓冲 1
  cube_texture[TEX_QUAD] = initFrameBuffer(FRAME_BUFF_ID_1, "texture0");
}

void renderCubes() {
}

void renderPlane() {
}

void renderGrass() {
}

void renderSkybox() {
}