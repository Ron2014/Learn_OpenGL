#include <iostream>
#include <vector>

#include "global.h"
#include <GLFW/glfw3.h>
using namespace glm;
using namespace std;

#define BUFF_LEN 3
extern unsigned int VBO[BUFF_LEN]={0}, VAO[BUFF_LEN]={0};

const char *texture_data[TEX_COUNT][2] = {
  {"metal.png", "texture0"},
  {"marble.jpg", "texture0"},
  {"grass.png", "texture0"},
};
extern Texture2D *cube_texture[TEX_COUNT] = {nullptr};

vector< vector<float> > all_vertices = {
  // plane
  {
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f				
  },
  // cube
  {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  },
  // grass
  {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f,
  },
};

glm::vec3 cube_positions[] = {
  glm::vec3( -1.0f, 0.0f, -1.0f), 
  glm::vec3( 2.0f, 0.0f, 0.0f), 
};

glm::vec3 grass_positions[] = {
  glm::vec3(-1.5f,  0.0f, -0.48f),
  glm::vec3( 1.5f,  0.0f,  0.51f),
  glm::vec3( 0.0f,  0.0f,  0.7f),
  glm::vec3(-0.3f,  0.0f, -2.3f),
  glm::vec3( 0.5f,  0.0f, -0.6f),
};

void cleanCubeData() {
  for (int i=0; i<TEX_COUNT; i++) {
    if(cube_texture[i]) delete cube_texture[i];
    cube_texture[i] = nullptr;
  }
  if (VAO[0]) glDeleteVertexArrays(BUFF_LEN, VAO);
  if (VBO[0]) glDeleteBuffers(BUFF_LEN, VBO);
}

void initCubeData() {
  cleanCubeData();

  glGenVertexArrays(BUFF_LEN, VAO);      // 第二个参数实际上表示一个数组, 第一个参数表示数组大小.
  glGenBuffers(BUFF_LEN, VBO);           // 所以如果是一个 unsigned int 得用&, 如 glGenVertexArrays(1, &VBO);

  for (int i=0; i<BUFF_LEN; i++) {
    cout << "Main::genBuffer ------" << VAO[i] << " " << VBO[i] << all_vertices[i].size() << endl;
    glBindVertexArray(VAO[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
    glBufferData(GL_ARRAY_BUFFER, all_vertices[i].size()*sizeof(float), &(all_vertices[i][0]), GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
  }

  // unbind
  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  for (int i=0; i<TEX_COUNT; i++) {
    if (i==IDX_GRASS)
      cube_texture[i] = new Texture2D(texture_data[i][0], texture_data[i][1], 0, GL_CLAMP_TO_EDGE);
    else
      cube_texture[i] = new Texture2D(texture_data[i][0], texture_data[i][1]);
  }
}

void renderCubes() {
  glBindVertexArray(VAO[IDX_CUBE]);
  
  for (int i=0; i<(sizeof(cube_positions)/sizeof(glm::vec3)); i++) {
    shader[IDX_CUBE]->use();  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube_texture[TEX_CUBE]->ID);
    shader[IDX_CUBE]->setInt(cube_texture[TEX_CUBE]->uniform_name, 0);

    glm::vec3 pos = cube_positions[i];
    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    // model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 1.0f, 0.0f));
    shader[IDX_CUBE]->setMatrix4("model", glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }

  Texture2D::reset();                   // use 完之后记得重置
}

void renderPlane() {
  glBindVertexArray(VAO[IDX_PLANE]);

  shader[IDX_PLANE]->use();
  cube_texture[TEX_PLANE]->use();
  shader[TEX_PLANE]->setInt(cube_texture[TEX_PLANE]->uniform_name, 0);

  glm::mat4 model(1.0f);
  shader[IDX_PLANE]->setMatrix4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  Texture2D::reset();                   // use 完之后记得重置
}

void renderGrass() {
  glBindVertexArray(VAO[IDX_GRASS]);
  
  for (int i=0; i<(sizeof(grass_positions)/sizeof(glm::vec3)); i++) {
    shader[IDX_GRASS]->use();  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube_texture[TEX_GRASS]->ID);
    shader[IDX_GRASS]->setInt(cube_texture[TEX_GRASS]->uniform_name, 0);

    glm::vec3 pos = grass_positions[i];
    for (int j=0; j<3; j++) {
      glm::mat4 model(1.0f);
      model = glm::translate(model, pos);
      model = glm::rotate(model, glm::radians(60.0f*j), glm::vec3(0.0f, 1.0f, 0.0f));
      model = glm::translate(model, vec3(-0.5f, 0.0f, 0.0f));
      shader[IDX_GRASS]->setMatrix4("model", glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  }

  Texture2D::reset();                   // use 完之后记得重置
}