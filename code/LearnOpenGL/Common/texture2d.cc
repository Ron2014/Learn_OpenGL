#include "texture2d.h"
#include "stb_image.h"

#include <iostream>
#include <sstream>
using namespace std;

unsigned int Texture2D::TEXTURE_UNIT_ID = GL_TEXTURE0;

Texture2D::Texture2D(const GLchar *texPath, GLenum wrapping, GLenum filter) {
  if (TEXTURE_UNIT_ID==GL_TEXTURE0) stbi_set_flip_vertically_on_load(true);     // 图片的y轴0.0坐标通常在顶部, 图像加载时帮助我们翻转y轴

  // 生成Texture并指定模式, 要放在文件加载之前.
  // 可以理解成, 先在显存种开辟存储空间, stbi_load 才会work
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);

  // 为当前绑定的纹理对象设置环绕、过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);   
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

  stringstream ss;
  ss << TEXTURE_PATH << "\\" << texPath;
  data = stbi_load(ss.str().c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    GLint imgFmt = GL_RGB;
    // note that the awesomeface.png has transparency and thus an alpha channel
    // so make sure to tell OpenGL the data type is of GL_RGBA
    // 如果不指定RGBA, png图片渲染不出来
    if (ss.str().find(".png") != string::npos)
      imgFmt = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, imgFmt, width, height, 0, imgFmt, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    cout << "Fail to load texture " << texPath << endl;
  }
  stbi_image_free(data);

  unitID = TEXTURE_UNIT_ID++;
}

Texture2D::~Texture2D() {
  if (ID) glDeleteTextures(1, &ID);
}

void Texture2D::use() {
  // cout << unitID << endl;
  glActiveTexture(unitID);
  glBindTexture(GL_TEXTURE_2D, ID);
}