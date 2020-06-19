#include <iostream>
#include <vector>
#include <map>

#include "global.h"
#include <GLFW/glfw3.h>
using namespace glm;
using namespace std;

#define BUFF_LEN 4
extern unsigned int VBO[BUFF_LEN]={0}, VAO[BUFF_LEN]={0};
extern unsigned int FRAME_BUFF_ID=0;

const char *texture_data[TEX_COUNT][2] = {
  {"metal.png",   "texture0"},
  {"marble.jpg",  "texture0"},
  {NULL,          "texture0"},
};
extern Texture2D *cube_texture[TEX_COUNT] = {nullptr};

vector< vector<float> > all_vertices = {
  // plane
  {
         5.0f, -0.5f,  5.0f,  2.0f, 2.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 0.0f,				
  },
  // cube
  {
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
  },
  // mirror
  {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f,
  },
  // quad
  {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
  },
};

glm::vec3 cube_positions[] = {
  glm::vec3( -1.0f, 0.0f, -1.0f), 
  glm::vec3(  2.0f, 0.0f,  0.0f), 
};

glm::vec3 monitor_positions[] = {
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
  if (VAO[0]) glDeleteVertexArrays(BUFF_LEN, VAO);
  memset(VAO, 0, sizeof(VAO[0])*BUFF_LEN);
  if (VBO[0]) glDeleteBuffers(BUFF_LEN, VBO);
  memset(VBO, 0, sizeof(VBO[0])*BUFF_LEN);
}

void initCubeData() {
  cleanCubeData();

  ///////////////////////////////////////////////////////////////////////////// 默认帧缓冲

  glGenVertexArrays(BUFF_LEN, VAO);      // 第二个参数实际上表示一个数组, 第一个参数表示数组大小.
  glGenBuffers(BUFF_LEN, VBO);           // 所以如果是一个 unsigned int 得用&, 如 glGenVertexArrays(1, &VBO);

  for (int i=0; i<BUFF_LEN-1; i++) {
    cout << "Main::genBuffer ------" << VAO[i] << " " << VBO[i] << all_vertices[i].size() << endl;
    glBindVertexArray(VAO[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
    glBufferData(GL_ARRAY_BUFFER, all_vertices[i].size()*sizeof(float), &(all_vertices[i][0]), GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    // texture
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
  }

  glBindVertexArray(VAO[IDX_QUAD]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[IDX_QUAD]);
  glBufferData(GL_ARRAY_BUFFER, all_vertices[IDX_QUAD].size()*sizeof(float), &(all_vertices[IDX_QUAD][0]), GL_STATIC_DRAW);

  // position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
  // texture
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));

  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);
  
  // 生成纹理
  for (int i=0; i<2; i++) {
    cube_texture[i] = new Texture2D(texture_data[i][0], texture_data[i][1]);
  }
  cube_texture[TEX_QUAD] = new Texture2D(WIN_WIDTH, WIN_HEIGHT, texture_data[TEX_QUAD][1]);
  ///////////////////////////////////////////////////////////////////////////// 自定义帧缓冲

  glGenFramebuffers(1, &FRAME_BUFF_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, FRAME_BUFF_ID);

  // 创建纹理附件
  // cube_texture[IDX_QUAD] = new Texture2D(WIN_WIDTH, WIN_HEIGHT, texture_data[IDX_QUAD][1]);

  // 将它附加到当前绑定的帧缓冲对象
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cube_texture[TEX_QUAD]->ID, 0);

  // 渲染缓冲对象附件
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);

  glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIN_WIDTH, WIN_HEIGHT);  
  
  // 将它附加到当前绑定的帧缓冲对象
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderCubes() {
  Shader *s = shader[IDX_CUBE];
  unsigned int v = VAO[IDX_CUBE];
  Texture2D *tex = cube_texture[TEX_CUBE];

  glBindVertexArray(v);

  // for (int i=0; i<(sizeof(cube_positions)/sizeof(glm::vec3)); i++) {
  for (glm::vec3 pos : cube_positions) {
    s->use();  
    tex->use();
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, tex->ID);
    s->setInt(tex->uniform_name, 0);

    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    // model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 1.0f, 0.0f));
    s->setMatrix4("model", glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 36);
    Texture2D::reset();
  }
}

void renderPlane() {
  Shader *s = shader[IDX_PLANE];
  unsigned int v = VAO[IDX_PLANE];
  Texture2D *tex = cube_texture[TEX_PLANE];

  glBindVertexArray(v);

  s->use();
  tex->use();
  // glActiveTexture(GL_TEXTURE0);
  // glBindTexture(GL_TEXTURE_2D, tex->ID);
  s->setInt(tex->uniform_name, 0);

  glm::mat4 model(1.0f);
  s->setMatrix4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  Texture2D::reset();
}