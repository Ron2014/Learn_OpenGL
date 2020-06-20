#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <string>
using namespace std;

#define VERTEX_DEFAULT "vertex_default.shader"
#define FRAGMENT_DEFAULT "fragment_default.shader"
#define GEOMETRY_DEFAULT "geometry_default.shader"
#define SHADER_PATH "E:\\GitHub\\Learn_OpenGL\\res\\shaders\\"


class Shader {
  public:
    unsigned int ID;    // shader program id

    // read file to load shader source
    Shader(const GLchar *vertexPath = VERTEX_DEFAULT, const GLchar *fragmentPath = FRAGMENT_DEFAULT, const GLchar *geometryPath = nullptr);
    ~Shader();
    
    // active
    void use();

    // uniform get
    bool getBool(const string &name ) const;
    int getInt(const string &name ) const;
    unsigned int getUint(const string &name ) const;
    float getFloat(const string &name ) const;

    // uniform set
    void setBool(const string &name, bool value, int idx=-1) const;
    void setInt(const string &name, int value, int idx=-1) const;
    void setUint(const string &name, unsigned int value, int idx=-1) const;
    void setFloat(const string &name, float value, int idx=-1) const;
    
    void setMatrix4(const string &name, GLfloat *value, int idx=-1) const;
    void setVec3(const string &name, GLfloat *value, int idx=-1) const;

    void setMatrix4(const string &name, const glm::mat4 &mat4, int idx=-1) const;
    void setVec2(const string &name, const glm::vec2 &value, int idx=-1) const;
    void setVec3(const string &name, const glm::vec3 &value, int idx=-1) const;
    void setVec4(const string &name, const glm::vec4 &value, int idx=-1) const;
};