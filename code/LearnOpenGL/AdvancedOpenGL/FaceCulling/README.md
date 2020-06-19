## 概念

面剔除 Face Culling

面向(Front Facing):

- 正向面(Front Face)
- 背向面(Back Face) 默认剔除

环绕顺序(Winding Order):

- 逆时针(Counter-clockwise) 默认正向面
- 顺时针(Clockwise)

剔除（Cull，丢弃）

## 步骤

### 启用

glEnable(GL_CULL_FACE);

### 指定正向面

glFrontFace(GLenum);

- GL_CCW  逆时针 默认
- GL_CW   顺时针 

### 指定要剔除的面

glCullFace(GLenum);

- GL_BACK：只剔除背向面。
- GL_FRONT：只剔除正向面。
- GL_FRONT_AND_BACK：剔除正向面和背向面。