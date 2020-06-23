#pragma once

#define OUTLINE_DEPTH

#include "model.h"
#include "camera.h"
#include "shader.h"
#include "texture2d.h"
#include <GLFW/glfw3.h>

#define SHADOW_W 4096
#define SHADOW_H 4096

extern unsigned int WIN_WIDTH;
extern unsigned int WIN_HEIGHT;
extern float NEAR_Z;
extern float FAR_Z;

extern enum {
  IDX_CUBE,
  IDX_PLANE,
  IDX_QUAD,

  IDX_DBUG,
  IDX_MODEL,
  IDX_OUTLINE_MODEL,
  IDX_LAMP,
  IDX_SIMPLE_DEPTH,
  IDX_CUBEMAP_DEPTH,
  SHADER_NUM,
};
extern Shader *shader[];

// shader中定义对应数量的灯
// #define POINT_LIGHT_NUM 4
#define POINT_LIGHT_NUM 1
extern glm::vec3 gPointLightPositions[][2];
extern Texture2D *tex_depth_point[];
extern glm::vec3 directLightDir;

extern unsigned int VBO[], VAO[];
extern vector<int> OBJ_IDXS;

extern enum {
  FB_DEPTH_DIRECT,
  FB_DEPTH_POINT,
  FB_NUM,
};
extern unsigned int FRAME_BUFFER[];
extern Texture2D *tex_fb[];

extern enum {
  TEX_DIFFUSE,
  TEX_SPECULAR,
  TEX_EMISSION,
  TEX_PLANE,
  TEX_COUNT,
};
extern Texture2D *cube_texture[];
extern Cubemaps *tex_skybox;

extern GLFWwindow* window;
extern Camera::Camera *camera;

extern Model **loading_models;
extern int model_count;
extern int selected_model;
extern glm::vec3 *model_pos;

extern float deltaTime;
extern float lastFrame;
extern float sunSpeed;

extern void initShaders();
extern void switchModels();
extern void moveModel(unsigned int, float);
extern void switchFullScreen();

extern int shaderShadow;

extern bool rolling;
extern bool drawNormal;
extern bool gamma;
extern int dbgShadow;

extern unsigned int amount;
extern glm::mat4 *modelMatrices;