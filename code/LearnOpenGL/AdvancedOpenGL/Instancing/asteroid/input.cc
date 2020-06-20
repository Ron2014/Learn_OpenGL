#include <iostream>
using namespace std;

#include "global.h"
#include "camera.h"

#include <vector>
#include <map>
#include <GLFW/glfw3.h>

#define DEFAULT_SENSITIVE 0.2f
#define FORBID_SENSITIVE 0.21f

float lastX=WIN_WIDTH*0.5f, lastY=WIN_HEIGHT*0.5f;
float sensitive = DEFAULT_SENSITIVE;
bool firstMouse = true;
bool fillType = true;
map<unsigned int, float> key_lastFrame;

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
  cout << currentFrame << " " << key_lastFrame[key] << endl;
  if (currentFrame - key_lastFrame[key] > sensitive) {
    key_lastFrame[key] = currentFrame;
    return true;
  }
  return false;
}

bool isKeyPressed(GLint key) {
  return glfwGetKey(window, key) == GLFW_PRESS && checkKeySensitive(key);
}

bool isKeyHolding(GLint key) {
  return glfwGetKey(window, key) == GLFW_PRESS;
}

void processInput(GLFWwindow *window)
{
  if(isKeyPressed(GLFW_KEY_ESCAPE))
    glfwSetWindowShouldClose(window, true);           // response to ESC

  if(isKeyHolding(GLFW_KEY_W))
    camera->ProcessKeyboard(Camera::FORWARD, deltaTime);
  if(isKeyHolding(GLFW_KEY_S))
    camera->ProcessKeyboard(Camera::BACKWARD, deltaTime);
  if(isKeyHolding(GLFW_KEY_A))
    camera->ProcessKeyboard(Camera::LEFT, deltaTime);
  if(isKeyHolding(GLFW_KEY_D))
    camera->ProcessKeyboard(Camera::RIGHT, deltaTime);
  if(isKeyHolding(GLFW_KEY_Q))
    camera->ProcessKeyboard(Camera::RISE, deltaTime);
  if(isKeyHolding(GLFW_KEY_E))
    camera->ProcessKeyboard(Camera::FALL, deltaTime);

  
  if(isKeyPressed(GLFW_KEY_SPACE)) {
    fillType = !fillType;
    if (fillType) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  if(isKeyPressed(GLFW_KEY_TAB)) {
    switchModels();
  }

  if(isKeyPressed(GLFW_KEY_ENTER)) {
    // <Enter>: reload shader
    if (isKeyHolding(GLFW_KEY_LEFT_CONTROL)) {
      if (sensitive > DEFAULT_SENSITIVE) {
        sensitive = DEFAULT_SENSITIVE;
        return;
      }
      cout << "b full" << endl;
      switchFullScreen();
      cout << "a full" << endl;
      sensitive = FORBID_SENSITIVE;
    } else initShaders();
  }

  if(isKeyPressed(GLFW_KEY_PAUSE)) {
    // <Enter>: reload shader
    rolling = !rolling;
  }

  if(isKeyPressed(GLFW_KEY_N)) {
    // <Enter>: reload shader
    drawNormal = !drawNormal;
  }
}