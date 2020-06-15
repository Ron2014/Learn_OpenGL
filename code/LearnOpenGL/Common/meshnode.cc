#include "meshnode.h"
#include <assimp/material.h>
#include <map>

Mesh::Mesh(const vector<Vertex> &vertices, const vector<unsigned int> &indices, const vector<Texture2D *> &textures) {
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  setupMesh();
}

void Mesh::Draw(Shader *shader) {
#ifdef __DEBUG_DRAW
  cout << "Mess draw ------" << indices.size() << endl;
#endif
  map<unsigned int, unsigned int> texture_ids;
  texture_ids[aiTextureType_DIFFUSE] = 0;
  texture_ids[aiTextureType_SPECULAR] = 0;
  texture_ids[aiTextureType_HEIGHT] = 0;
  texture_ids[aiTextureType_AMBIENT] = 0;

  int array_idx = 0;
  for (Texture2D *tex : textures) {
    int type_idx = texture_ids[tex->type]++;
#ifdef __DEBUG_DRAW
    cout << Texture2D::TEXTURE_UNIT_ID << " " << tex->uniform_name << type_idx << " " << array_idx << endl;
#endif
    tex->use();
    shader->setInt(tex->uniform_name+to_string(type_idx), array_idx++);
  }
  
  // 绘制网格
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  // glDrawArrays(GL_TRIANGLES, 0, vertices.size());

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);
  // always good practice to set everything back to defaults once configured.
  Texture2D::reset();     // 位置0留空, 不绑定任何texture
}

void Mesh::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  cout << "Mesh::setupMesh ------" << VAO << " " << VBO << " " << EBO << endl;
  glBindVertexArray(VAO);   // -----------------

  // 数据导入缓冲区
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

#ifdef CALC_TANGENT_SPACE
  // vertex tangent
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

  // vertex bitangent
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
#endif

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);     // -----------------
}

void Mesh::Clean() { 
  cout << "Mesh::Clean ------" << VAO << " " << VBO << " " << EBO << endl;
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}
