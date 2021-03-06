#pragma once

#define OUTLINE_DEPTH

#include "camera.h"
#include "shader.h"
#include "texture2d.h"
#include <GLFW/glfw3.h>

extern unsigned int WIN_WIDTH;
extern unsigned int WIN_HEIGHT;
extern float NEAR_Z;
extern float FAR_Z;

extern enum {
  IDX_LAMP,
  IDX_QUAD,
  SHADER_NUM,
};
extern Shader *shader[];

extern unsigned int VBO[], VAO[];
extern unsigned int FRAME_BUFF_ID_0;
extern unsigned int FRAME_BUFF_ID_1;

extern enum {
  TEX_MSAA,
  TEX_QUAD,
  TEX_COUNT,
};
extern Texture2D *cube_texture[];

extern Cubemaps *tex_skybox;

extern GLFWwindow* window;
extern Camera::Camera *camera;

extern float deltaTime;
extern float lastFrame;

extern void initShaders();
extern void switchModels();
extern void switchFullScreen();

extern bool rolling;
extern bool drawNormal;

extern unsigned int amount;
extern glm::mat4 *modelMatrices;