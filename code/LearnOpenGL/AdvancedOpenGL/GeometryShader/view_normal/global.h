#pragma once

#define OUTLINE_DEPTH
#include "shader.h"
#include "texture2d.h"

extern unsigned int WIN_WIDTH;
extern unsigned int WIN_HEIGHT;
extern float NEAR_Z;
extern float FAR_Z;
extern bool drawNormal;

extern enum {
  IDX_CUBE,
  IDX_SKYBOX,
  IDX_LAMP,
  IDX_MODEL,
  IDX_OUTLINE_CUBE,
  IDX_OUTLINE_MODEL,
  IDX_NORMAL,
  SHADER_NUM,
};
extern Shader *shader[];

extern unsigned int VBO[], VAO[];

extern enum {
  TEX_DIFFUSE,
  TEX_SPECULAR,
  TEX_EMISSION,
  TEX_COUNT,
};
extern Texture2D *cube_texture[];

extern Cubemaps *tex_skybox;