#include "bufferutil.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
using namespace std;

/////////////////////////////////////////////////////////////////////////////// 顶点缓冲 & 顶点属性

/**
 * size 顶点数据的大小, 单位为每个分量 (一般每个分量都是float)
 * data 顶点数据
 * attrsize 表示顶点属性的分布规则
*/
void initBuffer(unsigned int &vao,
  unsigned int &vbo,
  const unsigned int size,
  const void* data,
  const vector<unsigned int> &attrsize) {
    
  unsigned int floatSize = sizeof(float);
  
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size*floatSize, data, GL_STATIC_DRAW);
  
  unsigned int offset = 0;
  for (auto v: attrsize)
    offset += v;

  unsigned int boffset = 0;
  for (int i=0; i < attrsize.size(); i++) {
    glVertexAttribPointer(i, attrsize[i], GL_FLOAT, GL_FALSE, offset*floatSize, (void*)(boffset));
    glEnableVertexAttribArray(i);
    boffset += attrsize[i] * floatSize;
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}


/////////////////////////////////////////////////////////////////////////////// 帧缓冲

// 帧缓冲: 一般纹理
Texture2D *initFrameBuffer(unsigned int &frameBufferID, string uniform_name) {
  glGenFramebuffers(1, &frameBufferID);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

  // 创建纹理附件
  Texture2D *tex = new TextureAttach(WIN_WIDTH, WIN_HEIGHT, uniform_name);

  // 绑定纹理附件
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->ID, 0);                    // GL_COLOR_ATTACHMENT0

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

  return tex;
}

// 帧缓冲: MSAA纹理
Texture2D *initFrameBufferSample(unsigned int &frameBufferID, const int sample, string uniform_name) {
  glGenFramebuffers(1, &frameBufferID);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

  // 创建纹理附件
  Texture2D *tex = new TextureAttachSample(WIN_WIDTH, WIN_HEIGHT, sample);

  // 绑定纹理附件
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex->ID, 0);          // GL_TEXTURE_2D_MULTISAMPLE

  // 渲染缓冲对象附件
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);

  glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, sample, GL_DEPTH24_STENCIL8, WIN_WIDTH, WIN_HEIGHT);        // glRenderbufferStorageMultisample
  
  // 将它附加到当前绑定的帧缓冲对象
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return tex;
}

// 帧缓冲: 深度贴图纹理
Texture2D *initFrameBufferDepth(unsigned int &frameBufferID, string uniform_name, int width, int height) {
  glGenFramebuffers(1, &frameBufferID);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

  // 创建纹理附件
  Texture2D *tex = new TextureAttachDepth(width, height, uniform_name, GL_CLAMP_TO_BORDER);

  // 绑定纹理附件
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex->ID, 0);                         // GL_DEPTH_ATTACHMENT

/*
  // 渲染缓冲对象附件
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);

  glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIN_WIDTH, WIN_HEIGHT);  
  
  // 将它附加到当前绑定的帧缓冲对象
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
*/
  // 不需要颜色缓冲
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return tex;
}

// 帧缓冲: 立方体深度贴图纹理
Cubemaps *initFrameBufferCubemapDepth(unsigned int &frameBufferID, string uniform_name, int width, int height) {
  glGenFramebuffers(1, &frameBufferID);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

  // 创建纹理附件
  Cubemaps *tex = new CubemapsAttachDepth(width, height, uniform_name, GL_CLAMP_TO_EDGE);

  // 绑定纹理附件
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex->ID, 0);

/*
  // 渲染缓冲对象附件
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);

  glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIN_WIDTH, WIN_HEIGHT);  
  
  // 将它附加到当前绑定的帧缓冲对象
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
*/
  // 不需要颜色缓冲
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return tex;
}

// 帧缓冲: 浮点颜色帧缓冲
Texture2D *initFrameBufferColor(unsigned int &frameBufferID, string uniform_name) {
  glGenFramebuffers(1, &frameBufferID);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

  // 创建纹理附件
  Texture2D *tex = new TextureAttachColor(WIN_WIDTH, WIN_HEIGHT, uniform_name);

  // 绑定纹理附件
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->ID, 0);   

  // 渲染缓冲对象附件
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);

  glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIN_WIDTH, WIN_HEIGHT);
  
  // 将它附加到当前绑定的帧缓冲对象
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
 
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return tex;
}

