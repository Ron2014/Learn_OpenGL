#pragma once

#include <glad/glad.h>
#include <string>
using namespace std;

#define VERTEX_PATH "E:\\GitHub\\Learn_OpenGL\\res\\shaders\\default_vertex_shader.shader"
#define FRAGMENT_PATH "E:\\GitHub\\Learn_OpenGL\\res\\shaders\\default_fragment_shader.shader"


class Shader {
  public:
    unsigned int ID;    // shader program id

    // read file to load shader source
    Shader(const GLchar *vertexPath = VERTEX_PATH, const GLchar *fragmentPath = FRAGMENT_PATH);
    ~Shader();
    
    // active
    void use();

    // uniform set
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setUint(const string &name, unsigned int value) const;
    void setFloat(const string &name, float value) const;
};