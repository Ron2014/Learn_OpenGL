#include "global.h"
#define CUBEMAP_ATTRIB_OFFSET 6

void cubemapAttrib(unsigned int vao) {
  glBindVertexArray(vao);

  GLuint mat4Size = sizeof(glm::mat4);
  GLuint vec4Size = sizeof(glm::vec4);

  for (int j=0; j<FACE_SIZE; j++) {
    for (int k=0; k<4; k++) {
      glEnableVertexAttribArray(CUBEMAP_ATTRIB_OFFSET+j*4+k);
      glVertexAttribPointer(CUBEMAP_ATTRIB_OFFSET+j*4+k, 4, GL_FLOAT, GL_FALSE, mat4Size, (void*)((j*4+k)*vec4Size));
      glVertexAttribDivisor(CUBEMAP_ATTRIB_OFFSET+j*4+k, 0);
    }
  }

  glBindVertexArray(0);
}

void update(float updateTime) {
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 0.1f, far_plane = 100.0f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    
    glm::mat4 trans(1.0f);
    if(rolling) trans = glm::rotate(trans, glm::radians(updateTime*sunSpeed), glm::vec3(0.0f, 0.0f, 1.0f));
    directLightDir = glm::vec3(trans * glm::vec4(directLightDir, 1.0f));

    lightView = glm::lookAt(-directLightDir, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    
    // near_plane = 1.0f, far_plane = 25.0f;
    if(!orthogonal) lightProjection = glm::perspective(glm::radians(90.0f), (float)SHADOW_W/SHADOW_H, near_plane, far_plane);
    glm::mat4 pointMatrix[6];
    trans = glm::mat4(1.0f);
    if(rolling) trans = glm::rotate(trans, glm::radians(updateTime*sunSpeed), glm::vec3(0.0f, 0.0f, 1.0f));
    pointLightPos = glm::vec3(trans * glm::vec4(pointLightPos, 1.0f));
    pointMatrix[0] = lightProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)); 
    pointMatrix[1] = lightProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)); 
    pointMatrix[2] = lightProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)); 
    pointMatrix[3] = lightProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)); 
    pointMatrix[4] = lightProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)); 
    pointMatrix[5] = lightProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)); 

    shader[IDX_CUBEMAP_DEPTH]->setVec3("pointLightPos", pointLightPos);
    shader[IDX_CUBEMAP_DEPTH]->setFloat("far_plane", far_plane);
    for (int i=0; i<FACE_SIZE; i++) {
      shader[IDX_CUBEMAP_DEPTH]->setMatrix4("pointLightMatrix", pointMatrix[i], i);
    }

    for (int i : OBJ_IDXS) {
      shader[i]->setFloat("near_plane", near_plane);
      shader[i]->setFloat("far_plane", far_plane);
      shader[i]->setMatrix4("lightSpaceMatrix", lightSpaceMatrix);
      shader[i]->setVec3("directLight.direction", directLightDir);
      shader[i]->setVec3("pointLight.position", pointLightPos);
    }
}