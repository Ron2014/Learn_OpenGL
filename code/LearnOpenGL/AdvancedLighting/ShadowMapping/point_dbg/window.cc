#include <iostream>
using namespace std;

#include "global.h"

#include <sstream>
#include <vector>
#include <map>

void cleanWindow();
void initWindow();
void switchFullScreen();
void drawScene();
void drawQuad();

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void cleanCubeData();
void initCubeData();
void renderCubes();
void renderPlane();
void renderGrass();

void cleanShader();

void renderCamera();
void renderSkybox();
void renderPointLights();

void cleanModels();
void initModels(int n, char *path[]);
void renderModels();

#define DEFAULT_SCR_W 800
#define DEFAULT_SCR_H 600

extern unsigned int WIN_WIDTH = DEFAULT_SCR_W;
extern unsigned int WIN_HEIGHT = DEFAULT_SCR_H;

extern float NEAR_Z = 0.1f;
extern float FAR_Z = 1000.0f;
extern bool genShadow = false;
extern float sunSpeed = 1.0f;

GLFWwindow* window = nullptr;
Camera::Camera *camera = nullptr;

bool fullScreen = false;
extern float deltaTime = 0.0f;
extern float lastFrame = 0.0f;

int main(int argc, char *argv[]) {
  glfwInit();
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // two lines tell the version of OpenGL is 3.3
  // 用 Lab/Test/version.cc 工具查看OpenGL版本

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // will use core-profile
  // 我们只会用到OpenGL的子集，无需向后兼容的特性

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   // open in Mac OS X
#endif

  initWindow();


  // render loop:
  while(!glfwWindowShouldClose(window))
  {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    processInput(window);

    // render
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);       // 黑色空间
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 0.1f, far_plane = 100.0f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    
    glm::mat4 trans(1.0f);
    if(rolling) trans = glm::rotate(trans, glm::radians(deltaTime*sunSpeed), glm::vec3(0.0f, 0.0f, 1.0f));
    directLightDir = glm::vec3(trans * glm::vec4(directLightDir, 1.0f));

    lightView = glm::lookAt(-directLightDir, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    
    stringstream pointMatrixName;
    glm::vec3 lookPoint;
    for (int i : OBJ_IDXS) {
      shader[i]->setMatrix4("directLightMatrix", lightSpaceMatrix);
      shader[i]->setVec3("directLight.direction", directLightDir);

      for (int j=0; j<POINT_LIGHT_NUM; j++) {
        // right
        pointMatrixName.str("");
        lookPoint = gPointLightPositions[j][0] + glm::vec3(1.0f, 0.0f, 0.0f);
        lightView = glm::lookAt(gPointLightPositions[j][0], lookPoint, glm::vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;
        shader[i]->setMatrix4("pointLightMatrix.right", lightSpaceMatrix, j);

        // left
        pointMatrixName.str("");
        lookPoint = gPointLightPositions[j][0] + glm::vec3(-1.0f, 0.0f, 0.0f);
        lightView = glm::lookAt(gPointLightPositions[j][0], lookPoint, glm::vec3(0.0f, -1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;
        shader[i]->setMatrix4("pointLightMatrix.left", lightSpaceMatrix, j);

        // top
        pointMatrixName.str("");
        lookPoint = gPointLightPositions[j][0] + glm::vec3(0.0f, 1.0f, 0.0f);
        lightView = glm::lookAt(gPointLightPositions[j][0], lookPoint, glm::vec3(0.0f, 0.0f, 1.0f));
        lightSpaceMatrix = lightProjection * lightView;
        shader[i]->setMatrix4("pointLightMatrix.top", lightSpaceMatrix, j);

        // bottom
        pointMatrixName.str("");
        lookPoint = gPointLightPositions[j][0] + glm::vec3(0.0f, -1.0f, 0.0f);
        lightView = glm::lookAt(gPointLightPositions[j][0], lookPoint, glm::vec3(0.0f, 0.0f, -1.0f));
        lightSpaceMatrix = lightProjection * lightView;
        shader[i]->setMatrix4("pointLightMatrix.bottom", lightSpaceMatrix, j);

        // front
        pointMatrixName.str("");
        lookPoint = gPointLightPositions[j][0] + glm::vec3(0.0f, 0.0f, 1.0f);
        lightView = glm::lookAt(gPointLightPositions[j][0], lookPoint, glm::vec3(0.0f, -1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;
        shader[i]->setMatrix4("pointLightMatrix.front", lightSpaceMatrix, j);

        // front
        pointMatrixName.str("");
        lookPoint = gPointLightPositions[j][0] + glm::vec3(0.0f, 0.0f, -1.0f);
        lightView = glm::lookAt(gPointLightPositions[j][0], lookPoint, glm::vec3(0.0f, -1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;
        shader[i]->setMatrix4("pointLightMatrix.back", lightSpaceMatrix, j);
      }
    }

    int org_w = WIN_WIDTH, org_h = WIN_HEIGHT;
    // 1. 首选渲染深度贴图
    glViewport(0, 0, SHADOW_W, SHADOW_H);
    glBindFramebuffer(GL_FRAMEBUFFER, FRAME_BUFFER[FB_DEPTH_POINT]);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    // 只绘制阴影计算的物体
    genShadow = true;
    renderPlane();
    renderCubes();
    renderModels();
    genShadow = false;

    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, org_w, org_h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // if (dbgShadow) {
      // 2. 将深度贴图展示出来
      // glDepthMask(GL_FALSE);
      glDepthFunc(GL_LEQUAL);
    
      glm::mat4 projection(1.0f);
      projection = glm::perspective(glm::radians(camera->FieldOfView), float(WIN_WIDTH)/glm::max((float)WIN_HEIGHT,0.01f), NEAR_Z, FAR_Z);
      glm::mat4 view = camera->GetViewMatrix();

      shader[IDX_DBUG]->setMatrix4("projection", projection);
      shader[IDX_DBUG]->setMatrix4("view", view);
      shader[IDX_DBUG]->setFloat("near_plane", near_plane);
      shader[IDX_DBUG]->setFloat("far_plane", far_plane);

      glBindVertexArray(VAO[IDX_CUBE]);
      Texture2D::use({tex_fb[FB_DEPTH_POINT]}, shader[IDX_DBUG]);

      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);

      // glDepthMask(GL_TRUE);
      glDepthFunc(GL_LESS);
    // } else {
      // drawScene();
    // }

    glfwSwapBuffers(window);              // double buffer switch
    glfwPollEvents();                     // keyboard/mouse event
  }

  delete camera;

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  cleanShader();
  cleanModels();
  cleanCubeData();

  glfwTerminate();
  return 0;
}

void drawScene() {
  //////////////////////////////// render camera & spotlight 手电筒
  renderCamera(); // 相机的信息, 作用于所有shader
      
  //////////////////////////////// render point light
  renderPointLights();

  //////////////////////////////// render skybox
  renderSkybox();

  //////////////////////////////// render plane
  renderPlane();

  //////////////////////////////// render cubes
  renderCubes();

  //////////////////////////////// render loading_model
  renderModels();
}

void switchFullScreen() {
  fullScreen = !fullScreen;

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  if(fullScreen) {
    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
  } else {
    glfwSetWindowMonitor(window, NULL, (mode->width-DEFAULT_SCR_W)>>1, (mode->height-DEFAULT_SCR_H)>>1, DEFAULT_SCR_W, DEFAULT_SCR_H, mode->refreshRate);
  }
}

void cleanWindow() {
  if (window) {
    glfwDestroyWindow(window);
    window = nullptr;
  }
  if (camera) {
    delete camera;
    camera = nullptr;
  }
}

void initWindow() {
  cleanWindow();

  window = glfwCreateWindow(DEFAULT_SCR_W, DEFAULT_SCR_H, __FILE__, NULL, NULL);

  if (!window){
      cout << "Failed to create GLFW window" << endl;
      glfwTerminate();
      exit(-1);
  }
  glfwMakeContextCurrent(window);         // 当前线程与window绑定

  // walk around
  glfwSetWindowSizeCallback(window, framebuffer_size_callback);
  // look around
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);

  // zoom in/out
  glfwSetScrollCallback(window, scroll_callback);

  // glad 必须在 windows 创建成功后初始化
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Failed to initialize GLAD" << endl;
    glfwTerminate();
    exit(-1);
  }

  camera = new Camera::Camera(0.0f);

  initShaders();
  initCubeData();
  initModels(__argc-1, __argv+1);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
  glStencilMask(0xFF); // 启用模板缓冲写入

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
}