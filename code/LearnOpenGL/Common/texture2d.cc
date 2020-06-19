#include "texture2d.h"
#include "stb_image.h"
#include "shader.h"

#include <iostream>
#include <sstream>
using namespace std;

unsigned int Texture2D::TEXTURE_UNIT_ID = 0;

Texture2D::Texture2D(string texPath, string uniform_name, GLuint type, GLenum wrapping, GLenum minFilter, GLenum magFilter): type(type), uniform_name(uniform_name) {
  // 生成Texture并指定模式, 要放在文件加载之前.
  // 可以理解成, 先在显存种开辟存储空间, stbi_load 才会work
  glGenTextures(1, &ID);
  if (texPath.find("\\")==string::npos)
    texPath = TEXTURE_PATH + texPath;

  data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    // note that the awesomeface.png has transparency and thus an alpha channel
    // so make sure to tell OpenGL the data type is of GL_RGBA
    // 如果不指定RGBA, png图片渲染不出来
    GLenum imgFmt = GL_RGB;
    if (nrChannels == 1)
        imgFmt = GL_RED;
    else if (nrChannels == 3)
        imgFmt = GL_RGB;
    else if (nrChannels == 4)
        imgFmt = GL_RGBA;

#ifdef __DEBUG_LOAD
    cout << "load texture ------" << texPath.c_str() << " " << nrChannels << endl;
#endif
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, imgFmt, width, height, 0, imgFmt, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
  } else {
    cout << "Fail to load texture " << texPath << endl;
  }
  stbi_image_free(data);
}

Texture2D::~Texture2D() {
#ifdef __DEBUG_LOAD
  cout << "~Texture2D:" << ID << endl;
#endif
  if (ID) glDeleteTextures(1, &ID);
}

void Texture2D::use() {
  glActiveTexture(TEXTURE_UNIT_ID+GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ID);
  TEXTURE_UNIT_ID ++;
}

// 在渲染帧结束后调用
void Texture2D::reset() {
  Texture2D::TEXTURE_UNIT_ID = 0;
  glActiveTexture(GL_TEXTURE0);
}