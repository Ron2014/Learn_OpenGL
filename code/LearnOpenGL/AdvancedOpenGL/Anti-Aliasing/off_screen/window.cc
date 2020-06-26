#include <iostream>
using namespace std;

#include "global.h"

#include <vector>
#include <map>

void cleanWindow();
void initWindow(int, char *[]);
void switchFullScreen();

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void cleanCubeData();
void initCubeData(int);
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

void fillFrameBuffer();
void dealWithMSAA();
void drawScreen();

#define DEFAULT_SCR_W 800
#define DEFAULT_SCR_H 600

extern unsigned int WIN_WIDTH = DEFAULT_SCR_W;
extern unsigned int WIN_HEIGHT = DEFAULT_SCR_H;

extern float NEAR_Z = 0.1f;
extern float FAR_Z = 1000.0f;

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

  initWindow(argc, argv);


  // render loop:
  while(!glfwWindowShouldClose(window))
  {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    processInput(window);
    
    // 第一处理阶段(Pass)
    // 绘制自定义帧缓冲0(MSAA)
    fillFrameBuffer();

    // 第二处理阶段
    // 帧缓冲0填充帧缓冲1
    dealWithMSAA();

    // 第三处理阶段
    // 绘制屏幕四边形
    drawScreen();

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

void fillFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, FRAME_BUFF_ID_0);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 我们现在不使用模板缓冲

    //////////////////////////////// render camera & spotlight 手电筒
    renderCamera(); // 相机的信息, 作用于所有shader
    
    //////////////////////////////// render loading_model
    renderModels();
    
    ////////////////////////////////
    renderPlane();
    renderCubes();
        
    //////////////////////////////// render point light
    renderPointLights();
}

void dealWithMSAA() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, FRAME_BUFF_ID_0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FRAME_BUFF_ID_1);
    glBlitFramebuffer(0, 0, WIN_WIDTH, WIN_HEIGHT, 0, 0, WIN_WIDTH, WIN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void drawScreen() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO[IDX_QUAD]);

    shader[IDX_QUAD]->use();  
    cube_texture[TEX_QUAD]->use();
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, cube_texture[TEX_QUAD]->ID);
    shader[IDX_QUAD]->setInt(cube_texture[TEX_QUAD]->uniform_name, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    Texture2D::reset();
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

void initWindow(int argc, char *argv[]) {
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
  int sample = 4;
  if (argc>1) sample = atoi(argv[1]);
  initShaders();
  initCubeData(sample);
  initModels(argc-1, argv+1);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);

  glfwWindowHint(GLFW_SAMPLES, sample);
  glEnable(GL_MULTISAMPLE); // enabled by default on some drivers, but not all so always enable to make sure
  
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
  glStencilMask(0xFF); // 启用模板缓冲写入

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}