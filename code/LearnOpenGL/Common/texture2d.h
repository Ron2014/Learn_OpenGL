#pragma once

#include <glad/glad.h>
#include <string>
using namespace std;

#define TEXTURE_PATH "E:\\GitHub\\Learn_OpenGL\\res\\images\\"
#define TEXTURE_DEFAULT "avengers.jpg"

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

  Texture2D(int width, int height, string uniform_name="", GLuint type=0, GLenum wrapping = GL_REPEAT, GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR, GLenum magFilter = GL_LINEAR);
  Texture2D(string texPath, string uniform_name="", GLuint type=0, GLenum wrapping = GL_REPEAT, GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR, GLenum magFilter = GL_LINEAR);
  ~Texture2D();

  void use();

  static void reset();
};