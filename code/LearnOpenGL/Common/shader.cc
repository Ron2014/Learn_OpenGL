#include "shader.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#define INFO_LEN 512

static bool checkError(const int objID, const char *msg) {
  int success;
  char info[INFO_LEN];
  glGetShaderiv(objID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(objID, INFO_LEN, NULL, info);
    printf("%s:\n", msg);
    printf("%s\n", info);
    return false;
  }
  return true;
}

static void createShader(GLenum shader_type, const char *shader_source, const char *file_name, vector<int> &shaders) {
  int objID = glCreateShader(shader_type);
  glShaderSource(objID, 1, &shader_source, NULL);
  glCompileShader(objID);

  char errMsg[INFO_LEN];
  sprintf(errMsg, "ERROR: Shader %s COMPILATION FAILED", file_name);
  if (checkError(objID, errMsg)) {
    shaders.push_back(objID);
  }
}

static int createProgram(vector<int> &shaders) {
  int shaderProgram = glCreateProgram();
  for (auto objID : shaders) {
    glAttachShader(shaderProgram, objID);
  }
  glLinkProgram(shaderProgram);

  if (checkError(shaderProgram, "ERROR: Shader Program LINKING FAILED")) {
    for (auto objID : shaders) {
      glDeleteShader(objID);
    }
  }
  return shaderProgram;
}

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
  ifstream vShaderFile;
  ifstream fShaderFile;
  vShaderFile.exceptions (ifstream::failbit | ifstream::badbit);
  fShaderFile.exceptions (ifstream::failbit | ifstream::badbit);

  // char buffer[INFO_LEN];
  stringstream buffer;
  vector<int> shaders;
  try {
    cout << vertexPath << endl;
    cout << fragmentPath << endl;

    buffer << SHADER_PATH << "\\" << vertexPath;
    vShaderFile.open(buffer.str().c_str(), ios::in);

    buffer.str("");
    buffer << SHADER_PATH << "\\" << fragmentPath;
    fShaderFile.open(buffer.str().c_str(), ios::in);

    // vShaderFile.read(buffer, INFO_LEN-1);
    buffer.str("");
    buffer << vShaderFile.rdbuf();
    cout << buffer.str() << endl;
    createShader(GL_VERTEX_SHADER, buffer.str().c_str(), vertexPath, shaders);

    buffer.str("");
    buffer << fShaderFile.rdbuf();
    cout << buffer.str() << endl;
    createShader(GL_FRAGMENT_SHADER, buffer.str().c_str(), fragmentPath, shaders);

    ID = createProgram(shaders);

    vShaderFile.close();
    fShaderFile.close();

  } catch(ifstream::failure e) {
    cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
  }
}

Shader::~Shader() {
  if (ID) glDeleteProgram(ID);
}

void Shader::use() {
  glUseProgram(ID);
}

// bool Shader::getBool(const string &name) const {
//     int location = glGetUniformLocation(ID, name.c_str());
//     glUseProgram(ID);
//     glUniform1i(location, (int)value);
// }

// int Shader::getInt(const string &name) const {
//     int location = glGetUniformLocation(ID, name.c_str());
//     glUseProgram(ID);
//     glUniform1i(location, value);
// }

// unsigned int Shader::getUint(const string &name) const {
//     int location = glGetUniformLocation(ID, name.c_str());
//     glUseProgram(ID);
//     glUniform1ui(location, value);
// }

// float Shader::getFloat(const string &name) const {
//     int location = glGetUniformLocation(ID, name.c_str());
//     glUseProgram(ID);
//     glUniform1f(location, value);
// }

void Shader::setBool(const string &name, bool value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    glUseProgram(ID);
    glUniform1i(location, (int)value);
}

void Shader::setInt(const string &name, int value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    glUseProgram(ID);
    glUniform1i(location, value);
}

void Shader::setUint(const string &name, unsigned int value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    glUseProgram(ID);
    glUniform1ui(location, value);
}

void Shader::setFloat(const string &name, float value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    glUseProgram(ID);
    glUniform1f(location, value);
}

void Shader::setMatrix4(const string &name, GLfloat *value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    glUseProgram(ID);
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}