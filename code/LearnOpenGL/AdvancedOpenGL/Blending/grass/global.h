#pragma once

#define OUTLINE_DEPTH
#include "shader.h"
#include "texture2d.h"

extern unsigned int WIN_WIDTH;
extern unsigned int WIN_HEIGHT;
extern float NEAR_Z;
extern float FAR_Z;

extern enum {
  IDX_PLANE,
  IDX_CUBE,
  IDX_GRASS,
  IDX_MODEL,
  IDX_LAMP,
  IDX_OUTLINE_CUBE,
  IDX_OUTLINE_MODEL,
  SHADER_NUM,
};
extern Shader *shader[];

extern unsigned int VBO[], VAO[];

extern enum {
  TEX_PLANE,
  TEX_CUBE,
  TEX_GRASS,
  TEX_COUNT,
};
extern Texture2D *cube_texture[];