#include <iostream>
using namespace std;

#include "global.h"

#include <sstream>
#include <vector>
#include <map>

void cleanWindow();
void initWindow();
void switchFullScreen();

void render();

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
extern int shaderShadow = 0;
extern float sunSpeed = 1.0f;
extern float FRAME_PER_SECOND = 60.f;

float SECOND_PER_FRAME = 1.0f/FRAME_PER_SECOND;

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

  float previous = glfwGetTime();
  float lag = 0.0;

  // render loop:
  while(!glfwWindowShouldClose(window)) {
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrame;
    previous = currentTime;
    lag += deltaTime;

    // input
    processInput(window);

    while (lag > SECOND_PER_FRAME) {
      update(SECOND_PER_FRAME);
      lag -= SECOND_PER_FRAME;
    }

    render();
    glfwSwapBuffers(window);              // double buffer switch
    glfwPollEvents();                     // keyboard/mouse event

    lastFrame = currentTime;
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

void render() {
    // render
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);       // 黑色空间
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    int org_w = WIN_WIDTH, org_h = WIN_HEIGHT;
    glViewport(0, 0, SHADOW_W, SHADOW_H);

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

    // 2. 渲染点光源深度贴图
    glBindFramebuffer(GL_FRAMEBUFFER, FRAME_BUFFER[FB_DEPTH_POINT]);
      glClear(GL_DEPTH_BUFFER_BIT);

      // glDisable(GL_CULL_FACE);
      glCullFace(GL_FRONT);
      // 只绘制阴影计算的物体
      shaderShadow = IDX_CUBEMAP_DEPTH;
      Texture2D::use({tex_fb[FB_DEPTH_POINT]}, shader[shaderShadow]);
      renderPlane();
      renderCubes();
      renderModels();

      glCullFace(GL_BACK);
      // glEnable(GL_CULL_FACE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, org_w, org_h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    switch (dbgShadow) {
      case 0: {
        // 绘制场景
        shaderShadow = 0;
        drawScene();

      } break;
      case 1: {
        // 将深度贴图展示出来
        Texture2D::use({tex_fb[FB_DEPTH_DIRECT]}, shader[IDX_QUAD]);
        glBindVertexArray(VAO[IDX_QUAD]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // glBindVertexArray(0);

      } break;
      case 2: {
        renderCamera();

        // 立方体深度贴图        
        glDisable(GL_CULL_FACE);
        glDepthFunc(GL_LEQUAL);
        // glDepthMask(GL_FALSE);
        glBindVertexArray(VAO[IDX_SKYBOX]);

        Texture2D::use({tex_fb[FB_DEPTH_POINT]}, shader[IDX_DBUG]);
        glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
        shader[IDX_DBUG]->setMatrix4("view", view);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        // glBindVertexArray(0);
        glEnable(GL_CULL_FACE);
        
      } break;
    };
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

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
}