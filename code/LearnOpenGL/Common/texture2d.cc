#include "stb_image.h"
#include "texture2d.h"
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
  TEXTURE_UNIT_ID = 0;
  glActiveTexture(GL_TEXTURE0);
}

void Texture2D::use(const vector<Texture2D *> &aTex, const Shader *shader) {
  for (int i=0; i<aTex.size(); i++) {
    Texture2D *tex = aTex[i];
    glActiveTexture(GL_TEXTURE0+i);
    glBindTexture(GL_TEXTURE_2D, tex->ID);
    if (!tex->uniform_name.empty() && shader) shader->setInt(tex->uniform_name, i);
  }
}

TextureAttach::TextureAttach(int width, int height, string uniform_name, GLenum minFilter, GLenum magFilter):uniform_name(uniform_name) {
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

TextureAttachDepth::TextureAttachDepth(int width, int height, string uniform_name, GLenum wrapping, GLenum minFilter, GLenum magFilter):uniform_name(uniform_name) {
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
  if (wrapping == GL_CLAMP_TO_BORDER) {
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  }
}

TextureAttachSample::TextureAttachSample(int width, int height, int samples, string uniform_name):uniform_name(uniform_name),sample(samples) {
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ID);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
}

TextureGamma::TextureGamma(string texPath, string uniform_name, GLuint type, GLenum wrapping, GLenum minFilter, GLenum magFilter): type(type){
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
    GLenum internalFormat = GL_RGB;
    GLenum dataFormat = GL_RGB;
    if (nrChannels == 1) {
      internalFormat = dataFormat = GL_RED;
    } else if (nrChannels == 3) {
      internalFormat = GL_SRGB;
      dataFormat = GL_RGB;
    } else if (nrChannels == 4) {
      internalFormat = GL_SRGB_ALPHA;
      dataFormat = GL_RGBA;
    }
#ifdef __DEBUG_LOAD
    cout << "load texture ------" << texPath.c_str() << " " << nrChannels << endl;
#endif
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
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

const char *faces[FACE_SIZE] = {
  "right.jpg",
  "left.jpg",
  "top.jpg",
  "bottom.jpg",
  "front.jpg",
  "back.jpg",
};

Cubemaps::Cubemaps(string texPath, string uniform_name, GLuint type, GLenum wrapping, GLenum minFilter, GLenum magFilter): type(type), uniform_name(uniform_name) {
  // 生成Texture并指定模式, 要放在文件加载之前.
  // 可以理解成, 先在显存种开辟存储空间, stbi_load 才会work
  if (texPath.find("\\")==string::npos)
    texPath = CUBEMAPS_PATH + texPath;

  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < FACE_SIZE; i++)
  {
      string fullpath = texPath + '\\' + faces[i];
      unsigned char *data = stbi_load( fullpath.c_str(), &width, &height, &nrChannels, 0);
      if (data)
      {
          glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          stbi_image_free(data);
      }
      else
      {
          std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
          stbi_image_free(data);
      }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapping);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapping);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapping);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
}

Cubemaps::~Cubemaps() {
#ifdef __DEBUG_LOAD
  cout << "~Cubemaps:" << ID << endl;
#endif
  if (ID) glDeleteTextures(1, &ID);
}

void Cubemaps::use() {
  glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

void Cubemaps::reset() {
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}