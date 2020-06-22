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
            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
            // Left face
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            // Right face
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
            // Bottom face
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            // Top face
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left   
  },
  // plane
  {
        // Positions          // Normals         // Texture Coords
        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
        -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
        - 25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
  },
  // quad
  {
    // 铺满屏幕
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
    glm::vec3( 0.0f, 1.5f, 0.0f ), 
    glm::vec3( 2.0f, 0.0f, 1.0f ),
    glm::vec3(-1.0f, 0.0f, 2.0f ),
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
  }

  // for (int i=0; i<POINT_LIGHT_NUM; i++) {
  //   tex_depth_point[i] = initFrameBufferDepth(FRAME_BUFFER_DEPTH, "texture0");
  // }
  // tex_fb[FB_DEPTH_DIRECT] = initFrameBufferDepth(FRAME_BUFFER[FB_DEPTH_DIRECT], "shadowMap", SHADOW_W, SHADOW_H);
  tex_fb[FB_DEPTH_MAP] = initFrameBufferCubemapDepth(FRAME_BUFFER[FB_DEPTH_MAP], "shadowMap", SHADOW_W, SHADOW_H);
  // tex_fb[FB_DEPTH_DIRECT] = initFrameBufferDepth(FRAME_BUFFER[FB_DEPTH_DIRECT], "shadowMap");
  // tex_fb[FB_QUAD] = initFrameBuffer(FRAME_BUFFER[FB_QUAD], "texture0");
}

void renderCubes() {
  glBindVertexArray(VAO[IDX_CUBE]);

  int offset = 0;
  if (gamma) offset = TEX_COUNT;
  int shaderId = IDX_CUBE;
  if (genShadow) shaderId = IDX_CUBEMAP_DEPTH;

  Texture2D::use({
    tex_fb[FB_DEPTH_MAP],
    cube_texture[TEX_DIFFUSE + offset], 
    cube_texture[TEX_SPECULAR + offset], 
    cube_texture[TEX_EMISSION + offset], 
    }, shader[shaderId]);

  for (int i=0; i<(sizeof(cube_positions)/sizeof(glm::vec3)); i++) {
    glm::vec3 pos = cube_positions[i];
    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    if (rolling) model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 1.0f, 0.0f));
    shader[shaderId]->setMatrix4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
}

void renderPlane() {
  glBindVertexArray(VAO[IDX_PLANE]);

  int offset = 0;
  if (gamma) offset = TEX_COUNT;
  int shaderId = IDX_PLANE;
  if (genShadow) shaderId = IDX_CUBEMAP_DEPTH;

  Texture2D::use({
    tex_fb[FB_DEPTH_MAP],
    cube_texture[TEX_PLANE + offset],
    }, shader[shaderId]);
  glm::mat4 model(1.0f);
  // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
  shader[shaderId]->setMatrix4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderGrass() {
}

void renderSkybox() {
}