#pragma once

#include <glad/glad.h>
#include <string>
using namespace std;

#define VERTEX_DEFAULT "vertex_default.shader"
#define FRAGMENT_DEFAULT "fragment_default.shader"
#define SHADER_PATH "E:\\GitHub\\Learn_OpenGL\\res\\shaders"


class Shader {
  public:
    unsigned int ID;    // shader program id

    // read file to load shader source
    Shader(const GLchar *vertexPath = VERTEX_DEFAULT, const GLchar *fragmentPath = FRAGMENT_DEFAULT);
    ~Shader();
    
    // active
    void use();

    // uniform set
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setUint(const string &name, unsigned int value) const;
    void setFloat(const string &name, float value) const;
};