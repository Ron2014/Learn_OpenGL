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

static void createShader(GLenum shader_type, const char* shader_source, vector<int> &shaders) {
  int objID = glCreateShader(shader_type);
  glShaderSource(objID, 1, &shader_source, NULL);
  glCompileShader(objID);

  char errMsg[INFO_LEN];
  sprintf(errMsg, "ERROR: Shader %d COMPILATION FAILED", shader_type);
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

    vShaderFile.open(vertexPath, ios::in);
    fShaderFile.open(fragmentPath, ios::in);

    // vShaderFile.read(buffer, INFO_LEN-1);
    buffer << vShaderFile.rdbuf();
    cout << buffer.str() << endl;
    createShader(GL_VERTEX_SHADER, buffer.str().c_str(), shaders);

    buffer.str("");
    buffer << fShaderFile.rdbuf();
    cout << buffer.str() << endl;
    createShader(GL_FRAGMENT_SHADER, buffer.str().c_str(), shaders);

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

void Shader::setFloat(const string &name, float value) const {
    int location = glGetUniformLocation(ID, name.c_str());
    glUseProgram(ID);
    glUniform1f(location, value);
}