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
  SHADER_NUM,
};
extern Shader *shader[];

extern unsigned int VBO[], VAO[];

extern enum {
  TEX_LAMP,
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