#include "global.h"

void renderDirectShadow() {
    // 1. 渲染定向光深度贴图
    glBindFramebuffer(GL_FRAMEBUFFER, FRAME_BUFFER[FB_DEPTH_DIRECT]);
      glClear(GL_DEPTH_BUFFER_BIT);
      glCullFace(GL_FRONT);

      // 只绘制阴影计算的物体
      shaderShadow = IDX_SIMPLE_DEPTH;
      Texture2D::use({tex_fb[FB_DEPTH_DIRECT]}, shader[shaderShadow]);
      // renderPlane();
      renderCubes();
      renderModels();

      glCullFace(GL_BACK);
}

void renderPointShadow() {
    // 2. 渲染点光源深度贴图
    glBindFramebuffer(GL_FRAMEBUFFER, FRAME_BUFFER[FB_DEPTH_POINT]);
      glClear(GL_DEPTH_BUFFER_BIT);

      // glDisable(GL_CULL_FACE);
      glCullFace(GL_FRONT);
      // 只绘制阴影计算的物体
      shaderShadow = IDX_CUBEMAP_DEPTH;
      Texture2D::use({tex_fb[FB_DEPTH_POINT]}, shader[shaderShadow]);

      // renderPlane();
      // renderBox();
      renderCubes();
      renderModels();

      glCullFace(GL_BACK);
      // glEnable(GL_CULL_FACE);
}

void renderColorBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, FRAME_BUFFER[FB_DEPTH_COLOR]);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      renderCamera();
      Texture2D::use({cube_texture[TEX_PLANE]}, shader[IDX_COLOR_BUF_W]);
      for (GLuint i = 0; i < lightPositions.size(); i++) {
        shader[IDX_COLOR_BUF_W]->setVec3("lights.position", lightPositions[i], i);
        shader[IDX_COLOR_BUF_W]->setVec3("lights.color", lightColors[i], i);
      }
      glm::mat4 model(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
      model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
      shader[IDX_COLOR_BUF_W]->setMatrix4("model", model);
      shader[IDX_COLOR_BUF_W]->setInt("reverse_normals", GL_TRUE);
      glBindVertexArray(VAO[IDX_CUBE]);
      glDrawArrays(GL_TRIANGLES, 0, 36);
}

void debugDirectShadow () {
  // 将深度贴图展示出来
  Texture2D::use({tex_fb[FB_DEPTH_DIRECT]}, shader[IDX_QUAD]);
  glBindVertexArray(VAO[IDX_QUAD]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  // glBindVertexArray(0);
} 

void debugPointShadow() {
  renderCamera();

  // 立方体深度贴图        
  glDisable(GL_CULL_FACE);
  glDepthFunc(GL_LEQUAL);
  // glDepthMask(GL_FALSE);
  glBindVertexArray(VAO[IDX_SKYBOX]);

  Texture2D::use({tex_fb[FB_DEPTH_POINT]}, shader[IDX_DBG_POINT_SHADOW]);
  glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
  shader[IDX_DBG_POINT_SHADOW]->setMatrix4("view", view);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  
  // glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
  // glBindVertexArray(0);
  glEnable(GL_CULL_FACE);
  
}