#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
using namespace std;

#define TEXTURE_PATH "E:\\GitHub\\Learn_OpenGL\\res\\images\\"
#define TEXTURE_DEFAULT "avengers.jpg"

class Shader;
class Texture2D {
public:
  static unsigned int TEXTURE_UNIT_ID;
  unsigned int ID;
  unsigned int type;
  string uniform_name;
  
  int width;
  int height;
  int nrChannels;
  unsigned char* data;

  Texture2D() {};
  Texture2D(string texPath,
    string uniform_name = "",
    GLuint type = 0,
    GLenum wrapping = GL_REPEAT,
    GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR,
    GLenum magFilter = GL_LINEAR);
  ~Texture2D();

  virtual void use();
  virtual void bind();
  static void reset();
  static void use(const vector<Texture2D *> &aTex, const Shader *shader = nullptr);
  static void use4model(const vector<Texture2D *> &aTex, const Shader *shader = nullptr, int offset=10);
};

class TextureAttach : public Texture2D {
public:
  TextureAttach() {};
  TextureAttach(int width, int height,
    string uniform_name = "",
    GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR,
    GLenum magFilter = GL_LINEAR);
};

class TextureAttachColor : public TextureAttach {
public:
  TextureAttachColor(int width, int height,
    string uniform_name = "",
    GLenum minFilter = GL_LINEAR,
    GLenum magFilter = GL_LINEAR);
};

class TextureAttachDepth : public Texture2D {
public:
  TextureAttachDepth(int width, int height,
    string uniform_name = "",
    GLenum wrapping = GL_REPEAT,
    GLenum minFilter = GL_NEAREST,
    GLenum magFilter = GL_NEAREST);
};

class TextureAttachSample : public TextureAttach {
public:
  int sample;
  TextureAttachSample(int width, int height, int sample,
    string uniform_name = "");
};

class TextureGamma : public Texture2D {
public:
  TextureGamma(string texPath,
    string uniform_name = "",
    GLuint type = 0,
    GLenum wrapping = GL_REPEAT,
    GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR,
    GLenum magFilter = GL_LINEAR);
};

#define CUBEMAPS_PATH "E:\\GitHub\\Learn_OpenGL\\res\\images\\"
#define CUBEMAPS_DEFAULT "skybox"
#define FACE_SIZE 6

class Cubemaps : public Texture2D {
public:
  Cubemaps(string texPath = CUBEMAPS_DEFAULT,
    string uniform_name = "",
    GLuint type = 0,
    GLenum wrapping = GL_CLAMP_TO_EDGE,
    GLenum minFilter = GL_LINEAR,
    GLenum magFilter = GL_LINEAR);
  ~Cubemaps();

  virtual void use();
  virtual void bind();
  static void reset();
};

class CubemapsAttachDepth : public Cubemaps {
public:
  CubemapsAttachDepth(int width, int height,
    string uniform_name = "",
    GLenum wrapping = GL_REPEAT,
    GLenum minFilter = GL_NEAREST,
    GLenum magFilter = GL_NEAREST);
};
