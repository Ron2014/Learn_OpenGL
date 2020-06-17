#include "meshnode.h"
#include <assimp/material.h>
#include <map>

Mesh::Mesh(const vector<Vertex> &vertices, const vector<unsigned int> &indices, const vector<Texture2D *> &textures, const vec3 &center):centerPos(center) {
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  setupMesh();
}

/**
 * 
 * rendering
 * 
 * 绑定顺序是严格的:
 * 1. 绑定顶点数组(属性)对象 VAO
 * 2. 绑定顶点/索引缓冲 VBO/EBO
 * 必须先绑定数组对象, 再绑定缓冲区
 * 因为数组(属性)对象是对缓冲区字节数据分布的描述信息.
 * 
 * 解绑顺序是任意的的:
 * 1. 解绑顶点数组(属性)对象 VAO
 * 2. 解绑顶点/索引缓冲 VBO/EBO
 * 数据都绘制完了, 用什么顺序解绑都无所谓了
*/
void Mesh::Draw(Shader *shader) {
#ifdef __DEBUG_DRAW
  cout << "Mess draw ------" << indices.size() << endl;
#endif
  map<unsigned int, unsigned int> texture_ids;
  texture_ids[aiTextureType_DIFFUSE]  = 0;
  texture_ids[aiTextureType_SPECULAR] = 0;
  texture_ids[aiTextureType_HEIGHT]   = 0;
  texture_ids[aiTextureType_AMBIENT]  = 0;

  for (unsigned int i=0; i<textures.size(); i++) {
    Texture2D *tex = textures[i];
    int num = texture_ids[tex->type]++;

#ifdef __DEBUG_DRAW
    cout << Texture2D::TEXTURE_UNIT_ID << " " << tex->uniform_name << num << " " << i << endl;
#endif
    tex->use();
    shader->setInt(tex->uniform_name + to_string(num), i);
  }
  
  // 绘制网格
  // 索引缓冲区都不需要绑定的, 只需要顶点属性缓冲区的绑定即可
  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // always good practice to set everything back to defaults once configured.
  Texture2D::reset();     // 位置0留空, 不绑定任何texture
}

/**
 * 
 * loading
 * 
 * 绑定顺序是任意的:
 * 1. 绑定顶点数组(属性)对象 VAO
 * 2. 绑定顶点/索引缓冲 VBO/EBO
 * 前后均可
 * 
 * 解绑顺序是严格的:
 * 1. 解绑顶点数组(属性)对象 VAO
 * 2. 解绑顶点/索引缓冲 VBO/EBO
 * 因为解绑调用的也是 glBindBuffer, 如果顺序不对, 会覆盖掉已经绑好的缓冲区
*/
void Mesh::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

#ifdef __DEBUG_LOAD
  cout << "Mesh::setupMesh ------" << VAO << " " << VBO << " " << EBO << endl;
#endif

  // 数据导入缓冲区

  glBindVertexArray(VAO);   // -----------------
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

  // 顶点属性解析
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
  glEnableVertexAttribArray(2);

  // vertex tangent
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

  // vertex bitangent
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

  glBindVertexArray(0);     // -----------------
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Mesh::~Mesh() {
void Mesh::Clean() { 
#ifdef __DEBUG_LOAD
  cout << "Mesh::Clean ------" << VAO << " " << VBO << " " << EBO << endl;
#endif
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}
