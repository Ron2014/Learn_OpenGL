#include <iostream>
using namespace std;

#include "model.h"
#include "global.h"
#include "camera.h"

#include <vector>
#include <map>
#include <GLFW/glfw3.h>

extern bool gamma = false;
extern bool orthogonal = false;
extern int dbgShadow = 0;

extern bool hdr = true;
extern float exposure = 1.0f;

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
  // cout << currentFrame << " " << key_lastFrame[key] << endl;
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

  if (selected_model < 0) {
    unsigned int camera_direct = 0;
    if(isKeyHolding(GLFW_KEY_W)) camera_direct = Camera::FORWARD;
    if(isKeyHolding(GLFW_KEY_S)) camera_direct = Camera::BACKWARD;
    if(isKeyHolding(GLFW_KEY_A)) camera_direct = Camera::LEFT;
    if(isKeyHolding(GLFW_KEY_D)) camera_direct = Camera::RIGHT;
    if(isKeyHolding(GLFW_KEY_Q)) camera_direct = Camera::RISE;
    if(isKeyHolding(GLFW_KEY_E)) camera_direct = Camera::FALL;
    if (camera_direct) {
      float ratio = 1.0f;
      if (isKeyHolding(GLFW_KEY_LEFT_SHIFT)) {
        // speed up
        ratio = 2.0f;
      } else if (isKeyHolding(GLFW_KEY_LEFT_CONTROL)) {
        // speed slow
        ratio = 0.5f;
      }
      camera->ProcessKeyboard((Camera::Camera_Movement)camera_direct, deltaTime, ratio);
    }
  } else {
    unsigned int model_move = 0;
    if(isKeyHolding(GLFW_KEY_W)) model_move = Model::FORWARD;
    if(isKeyHolding(GLFW_KEY_S)) model_move = Model::BACKWARD;
    if(isKeyHolding(GLFW_KEY_A)) model_move = Model::LEFT;
    if(isKeyHolding(GLFW_KEY_D)) model_move = Model::RIGHT;
    if(isKeyHolding(GLFW_KEY_Q)) model_move = Model::RISE;
    if(isKeyHolding(GLFW_KEY_E)) model_move = Model::FALL;
    if (model_move) {
      float ratio = 1.0f;
      if (isKeyHolding(GLFW_KEY_LEFT_SHIFT)) {
        // speed up
        ratio = 2.0f;
      } else if (isKeyHolding(GLFW_KEY_LEFT_CONTROL)) {
        // speed slow
        ratio = 0.1f;
      }
      moveModel(model_move, ratio);
    }
  }
  
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
      // cout << "b full" << endl;
      switchFullScreen();
      // cout << "a full" << endl;
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

  if(isKeyHolding(GLFW_KEY_B)) {
    shader[IDX_TBN_PLANE]->setInt("blinn", 0);  
    shader[IDX_PLANE]->setInt("blinn", 0);
  } else {
    shader[IDX_TBN_PLANE]->setInt("blinn", 1);  
    shader[IDX_PLANE]->setInt("blinn", 1);
  }

  if(isKeyHolding(GLFW_KEY_G)) {
    shader[IDX_PLANE]->setInt("gamma", 1);
    shader[IDX_TBN_PLANE]->setInt("gamma", 1);  
    gamma = true;
  } else {
    shader[IDX_PLANE]->setInt("gamma", 0);
    shader[IDX_TBN_PLANE]->setInt("gamma", 0);  
    gamma = false;
  }

  if(isKeyPressed(GLFW_KEY_DELETE)) {
    if (dbgShadow) dbgShadow = 0;
    else {
      if (isKeyHolding(GLFW_KEY_1)) {
        dbgShadow = 1;
      } else if(isKeyHolding(GLFW_KEY_2)) {
        dbgShadow = 2;
      }
    }
  }

  if(isKeyPressed(GLFW_KEY_PAGE_UP)) {
    sunSpeed *= 2.0f;
    cout << "sun speed up " << sunSpeed << endl;
  }
  if(isKeyPressed(GLFW_KEY_PAGE_DOWN)) {
    sunSpeed /= 2.0f;
    sunSpeed = glm::max(sunSpeed, 1.0f);
    cout << "sun speed down " << sunSpeed << endl;
  }

  if(isKeyPressed(GLFW_KEY_P)) {
    if (selected_model>=0) {
      cout << model_pos[selected_model].x << "," << model_pos[selected_model].y << "," << model_pos[selected_model].z << endl;
    }
  }

  if(isKeyPressed(GLFW_KEY_O)) {
    orthogonal = !orthogonal;
  }

  if(isKeyPressed(GLFW_KEY_LEFT_BRACKET)) {
    exposure /= 2.0f;
    if (exposure < 1.0f) exposure = 1.0f;
    cout << "exposure " << exposure << endl;
  }
  if(isKeyPressed(GLFW_KEY_RIGHT_BRACKET)) {
    exposure *= 2.0f;
    cout << "exposure " << exposure << endl;
  }

  if(isKeyPressed(GLFW_KEY_H)) {
    hdr = !hdr;
    cout << "hdr " << (hdr?'1':'0') << endl;
  }
}