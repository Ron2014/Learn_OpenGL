#pragma once

#define OUTLINE_DEPTH
#include "camera.h"
#include "shader.h"
#include "texture2d.h"

extern unsigned int WIN_WIDTH;
extern unsigned int WIN_HEIGHT;
extern float NEAR_Z;
extern float FAR_Z;

extern enum {
  IDX_PLANE,
  IDX_CUBE,
  IDX_MONITOR,
  IDX_DBUG,
  IDX_MODEL,
  IDX_LAMP,
  IDX_OUTLINE_CUBE,
  IDX_OUTLINE_MODEL,
  SHADER_NUM,
};
extern Shader *shader[];

extern unsigned int VBO[], VAO[];
extern unsigned int FRAME_BUFF_ID;

extern enum {
  TEX_PLANE,
  TEX_CUBE,
  TEX_QUAD,
  TEX_COUNT,
};
extern Texture2D *cube_texture[];

extern Camera::Camera *camera;

extern glm::vec3 monitor_positions[5];