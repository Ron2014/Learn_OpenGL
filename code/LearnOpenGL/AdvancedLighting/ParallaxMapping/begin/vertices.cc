#include <iostream>
#include <vector>

#include "bufferutil.h"
#include "global.h"
#include "camera.h"

#include <GLFW/glfw3.h>
using namespace glm;
using namespace std;

extern unsigned int VBO[BUFF_LEN+1]={0}, VAO[BUFF_LEN+1]={0};
extern unsigned int FRAME_BUFFER[FB_NUM]={0};
extern Texture2D *tex_fb[FB_NUM] = {nullptr};
extern bool rolling = true;

const char *texture_data[TEX_COUNT][2] = {
  {"container2.png",          "material.diffuse"},
  {"container2_specular.png", "material.specular"},
  {"matrix.jpg",              "material.emission"},
  {"bricks2.jpg",           "material.diffuse"},
  {"bricks2_normal.jpg",    "material.normal"},
  {"bricks2_disp.jpg",      "material.displace"},
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
  // skybox
  {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  },
};

vector< vector<unsigned int> > all_attrib = {
  {3, 3, 2,},
  {3, 3, 2,},
  {3, 2,},
  {3, 2,},
  {3, 3, 2, 3, 3},
};

glm::vec3 cube_positions[] = {
  glm::vec3(  4.0f, -3.5f,  0.0),
  glm::vec3(  2.0f,  3.0f,  1.0),
  glm::vec3( -3.0f, -1.0f,  0.0),
  glm::vec3( -1.5f,  1.0f,  1.5),
  glm::vec3( -1.5f,  2.0f, -3.0),
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
  for (int i=0; i<FB_NUM; i++) {
    if(tex_fb[i]) delete tex_fb[i];
    tex_fb[i] = nullptr;
  }
  if (VAO[0]) glDeleteVertexArrays(BUFF_LEN+1, VAO);
  if (VBO[0]) glDeleteBuffers(BUFF_LEN+1, VBO);
}

void genTBN(int id) {
  vector<float> values = all_vertices[IDX_QUAD];
  vector<glm::vec3> pos;
  vector<glm::vec2> uv;

  for (int i : {0, 1, 3, 2}) {
    pos.push_back(glm::vec3(values[i*5+0], values[i*5+1], values[i*5+2]));
    uv.push_back(glm::vec2(values[i*5+3], values[i*5+4]));
  }

  // normal vector
  glm::vec3 nm(0.0, 0.0, 1.0);

  // calculate tangent/bitangent vectors of both triangles
  glm::vec3 tangent1, bitangent1;
  glm::vec3 tangent2, bitangent2;
  // - triangle 1
  glm::vec3 edge1 = pos[2-1] - pos[1-1];
  glm::vec3 edge2 = pos[3-1] - pos[1-1];
  glm::vec2 deltaUV1 = uv[2-1] - uv[1-1];
  glm::vec2 deltaUV2 = uv[3-1] - uv[1-1];

  GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

  tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
  tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
  tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
  tangent1 = glm::normalize(tangent1);

  bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
  bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
  bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
  bitangent1 = glm::normalize(bitangent1);

  // - triangle 2
  edge1 = pos[3-1] - pos[1-1];
  edge2 = pos[4-1] - pos[1-1];
  deltaUV1 = uv[3-1] - uv[1-1];
  deltaUV2 = uv[4-1] - uv[1-1];

  f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

  tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
  tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
  tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
  tangent2 = glm::normalize(tangent2);

  bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
  bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
  bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
  bitangent2 = glm::normalize(bitangent2);

  GLfloat quadVertices[] = {
      // Positions                        // normal         // TexCoords          // Tangent                          // Bitangent
      pos[1-1].x, pos[1-1].y, pos[1-1].z, nm.x, nm.y, nm.z, uv[1-1].x, uv[1-1].y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
      pos[2-1].x, pos[2-1].y, pos[2-1].z, nm.x, nm.y, nm.z, uv[2-1].x, uv[2-1].y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
      pos[3-1].x, pos[3-1].y, pos[3-1].z, nm.x, nm.y, nm.z, uv[3-1].x, uv[3-1].y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

      pos[1-1].x, pos[1-1].y, pos[1-1].z, nm.x, nm.y, nm.z, uv[1-1].x, uv[1-1].y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
      pos[3-1].x, pos[3-1].y, pos[3-1].z, nm.x, nm.y, nm.z, uv[3-1].x, uv[3-1].y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
      pos[4-1].x, pos[4-1].y, pos[4-1].z, nm.x, nm.y, nm.z, uv[4-1].x, uv[4-1].y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
  };

  initBuffer(VAO[id], VBO[id], sizeof(quadVertices), quadVertices, all_attrib[id]);
}

void initCubeData() {
  cleanCubeData();

  for (int i=0; i<BUFF_LEN; i++) {
    initBuffer(VAO[i], VBO[i], all_vertices[i].size(), &(all_vertices[i][0]), all_attrib[i]);
  }
  genTBN(IDX_TBN_PLANE);

  for (int i=0; i<TEX_COUNT; i++) {
    cube_texture[i] = new Texture2D(texture_data[i][0], texture_data[i][1]);
    cube_texture[i+TEX_COUNT] = new TextureGamma(texture_data[i][0], texture_data[i][1]);
  }

  tex_fb[FB_DEPTH_DIRECT] = initFrameBufferDepth(FRAME_BUFFER[FB_DEPTH_DIRECT], "shadowMap", SHADOW_W, SHADOW_H);
  tex_fb[FB_DEPTH_POINT] = initFrameBufferCubemapDepth(FRAME_BUFFER[FB_DEPTH_POINT], "shadowCubemap", SHADOW_W, SHADOW_H);

  tex_skybox = new Cubemaps("skybox", "skybox");
  // cout << "names:" << endl;
  // for (int i=0; i<FB_NUM; i++)
  //   cout << tex_fb[i]->uniform_name << endl;
}

void renderCubes() {
  glBindVertexArray(VAO[IDX_CUBE]);

  int offset = 0;
  if (gamma) offset = TEX_COUNT;
  int shaderId = IDX_CUBE;
  if (shaderShadow) shaderId = shaderShadow;

  Texture2D::use({
    tex_fb[FB_DEPTH_DIRECT],
    tex_fb[FB_DEPTH_POINT],
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

void renderBox() {
  // 这次的 Plane 其实是个Cube
  glBindVertexArray(VAO[IDX_CUBE]);

  int offset = 0;
  if (gamma) offset = TEX_COUNT;
  int shaderId = IDX_PLANE;
  if (shaderShadow) shaderId = shaderShadow;

  Texture2D::use({
    tex_fb[FB_DEPTH_DIRECT],
    tex_fb[FB_DEPTH_POINT],
    cube_texture[TEX_PLANE + offset],
    cube_texture[TEX_PLANE_NORMAL + offset],
    }, shader[shaderId]);

  // 画一个法线向内的立方体
  // 用来建立点光源投影
  shader[shaderId]->setInt("reverse_normals", 1);
  glDisable(GL_CULL_FACE); // Note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
  
  glm::mat4 model(1.0f);
  model = glm::scale(model, glm::vec3(25.0f));
  shader[shaderId]->setMatrix4("model", model);
    
  glDrawArrays(GL_TRIANGLES, 0, 36);

  shader[shaderId]->setInt("reverse_normals", 0);
  glEnable(GL_CULL_FACE);
}

void renderPlane() {
  glBindVertexArray(VAO[IDX_TBN_PLANE]);

  int offset = 0;
  if (gamma) offset = TEX_COUNT;
  int shaderId = IDX_TBN_PLANE;
  if (shaderShadow) shaderId = shaderShadow;

  Texture2D::use({
    tex_fb[FB_DEPTH_DIRECT],
    tex_fb[FB_DEPTH_POINT],
    cube_texture[TEX_PLANE + offset],
    cube_texture[TEX_PLANE_NORMAL + offset],
    cube_texture[TEX_PLANE_DISPLACE + offset],
    }, shader[shaderId]);
    
  glm::mat4 model(1.0f);
  // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
  shader[shaderId]->setMatrix4("model", model);
  shader[shaderId]->setVec3("pointLightPos", pointLightPos);
  shader[shaderId]->setFloat("height_scale", 1.0f);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderGrass() {
}

void renderSkybox() {
  // Texture2D::reset();
  // Cubemaps::reset();
  
  glDisable(GL_CULL_FACE);
  glDepthFunc(GL_LEQUAL);
  // glDepthMask(GL_FALSE);
  glBindVertexArray(VAO[IDX_SKYBOX]);
  Texture2D::use({tex_skybox}, shader[IDX_SKYBOX]);
  // 观察矩阵去掉平移数据
  glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
  shader[IDX_SKYBOX]->setMatrix4("view", view);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  // glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  // glBindVertexArray(0);
}