#include <iostream>
using namespace std;
// #define FULL_SCREEN

#include "global.h"
#include "camera.h"

#include <vector>
#include <map>
#include <GLFW/glfw3.h>

void cleanWindow();
void initWindow(int, char *[]);
void switchFullScreen();

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void cleanCubeData();
void initCubeData();
void renderCubes(bool rolling);

void cleanShader();
void initShaders();

void renderCamera(Camera::Camera *camera);
void renderSkybox(Camera::Camera *camera);
void renderPointLights();

void cleanModels();
void initModels(int n, char *path[]);
void renderModels();
void switchModels();

#define DEFAULT_SCR_W 800
#define DEFAULT_SCR_H 600

extern unsigned int WIN_WIDTH = DEFAULT_SCR_W;
extern unsigned int WIN_HEIGHT = DEFAULT_SCR_H;
extern float NEAR_Z = 0.1f;
extern float FAR_Z = 1000.0f;

GLFWwindow* window = nullptr;
Camera::Camera *camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX=WIN_WIDTH*0.5f, lastY=WIN_HEIGHT*0.5f;
float sensitive = 0.2f;
bool firstMouse = true;
bool fillType = true;
bool rolling = true;
bool drawNormal = false;
bool fullScreen = false;
map<unsigned int, float> key_lastFrame;

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

    // render
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);       // 黑色空间
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //////////////////////////////// render camera & spotlight 手电筒
    renderCamera(camera); // 相机的信息, 作用于所有shader
    
    //////////////////////////////// render skybox
    renderSkybox(camera);
    
    //////////////////////////////// render loading_model
    renderModels();
    
    ////////////////////////////////
    renderCubes(rolling);
        
    //////////////////////////////// render point light
    renderPointLights();

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

  initShaders();
  initCubeData();
  initModels(argc-1, argv+1);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
  glStencilMask(0xFF); // 启用模板缓冲写入

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  camera->ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if(firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
  lastX = xpos;
  lastY = ypos;
  
  // cout << xoffset << " " << yoffset << endl;
  camera->ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  cout << "view change " << width << " " << height << endl;
  glViewport(0, 0, width, height);

  WIN_WIDTH = width;
  WIN_HEIGHT = height;
}  

bool checkKeySensitive(unsigned int key) {
  float currentFrame = deltaTime + lastFrame;
  if (currentFrame - key_lastFrame[key] > sensitive) {
    key_lastFrame[key] = currentFrame;
    return true;
  }
  return false;
}

void switchFullScreen() {
  fullScreen = !fullScreen;

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  if(fullScreen) {
    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
  } else {
    glfwSetWindowMonitor(window, NULL, (mode->width-DEFAULT_SCR_W)>>1, (mode->height-DEFAULT_SCR_H)>>1, DEFAULT_SCR_W, DEFAULT_SCR_H, mode->refreshRate);
    glfwSetWindowAttrib(window, GLFW_DECORATED, GL_TRUE);
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GL_TRUE);
  }
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);           // response to ESC

  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera->ProcessKeyboard(Camera::FORWARD, deltaTime);
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera->ProcessKeyboard(Camera::BACKWARD, deltaTime);
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera->ProcessKeyboard(Camera::LEFT, deltaTime);
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera->ProcessKeyboard(Camera::RIGHT, deltaTime);
  if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    camera->ProcessKeyboard(Camera::RISE, deltaTime);
  if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    camera->ProcessKeyboard(Camera::FALL, deltaTime);

  
  if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && checkKeySensitive(GLFW_KEY_SPACE)) {
    fillType = !fillType;
    if (fillType) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && checkKeySensitive(GLFW_KEY_TAB)) {
    switchModels();
  }

  if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && checkKeySensitive(GLFW_KEY_ENTER)) {
    // <Enter>: reload shader
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && checkKeySensitive(GLFW_KEY_LEFT_CONTROL)) {
      switchFullScreen();
    } else initShaders();
  }

  if(glfwGetKey(window, GLFW_KEY_PAUSE) == GLFW_PRESS && checkKeySensitive(GLFW_KEY_PAUSE)) {
    // <Enter>: reload shader
    rolling = !rolling;
  }

  if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && checkKeySensitive(GLFW_KEY_N)) {
    // <Enter>: reload shader
    drawNormal = !drawNormal;
  }
}