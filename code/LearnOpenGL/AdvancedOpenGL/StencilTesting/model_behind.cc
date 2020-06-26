#include <iostream>
using namespace std;
#define OUTLINE_DEPTH

#include "model.h"
#include "shader.h"
#include "texture2d.h"
#include "camera.h"

#include <vector>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void cleanCubeData();
void initCubeData();
void cleanShader();
void initShaders();
void cleanModels();
void initModels(int n, char *path[]);

#define BUFF_LEN 1
unsigned int VBO[BUFF_LEN]={0}, VAO[BUFF_LEN]={0};

enum {
  IDX_CUBE,
  IDX_MODEL,
  IDX_LAMP,
  IDX_BEHIND_CUBE,
  IDX_OUTLINE_MODEL,
  SHADER_NUM,
};
Shader *shader[SHADER_NUM]={nullptr};

int OBJ_IDXS[] = {IDX_CUBE, IDX_MODEL};

Model **loading_models = nullptr;
int model_count = 0;
int selected_model = -1;

enum {
  TEX_DIFFUSE,
  TEX_SPECULAR,
  TEX_EMISSION,
  TEX_COUNT,
};

const char *cube_texture_data[TEX_COUNT][2] = {
  {"container2.png", "material.diffuse"},
  {"container2_specular.png", "material.specular"},
  {"matrix.jpg", "material.emission"},
};

Texture2D *cube_texture[TEX_COUNT] = {nullptr};

unsigned int WIN_WIDTH = 800;
unsigned int WIN_HEIGHT = 600;

Camera::Camera *camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX=WIN_WIDTH*0.5f, lastY=WIN_HEIGHT*0.5f;
float sensitive = 0.2f;
bool firstMouse = true;
bool fillType = true;

float gVertices[] = {
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0.0f, 0.0f,

  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
  
  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
};

glm::vec3 gCubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f), 
  glm::vec3( 2.0f,  5.0f, -15.0f), 
  glm::vec3(-1.5f, -2.2f, -2.5f),  
  glm::vec3(-3.8f, -2.0f, -12.3f),  
  glm::vec3( 2.4f, -0.4f, -3.5f),  
  glm::vec3(-1.7f,  3.0f, -7.5f),  
  glm::vec3( 1.3f, -2.0f, -2.5f),  
  glm::vec3( 1.5f,  2.0f, -2.5f), 
  glm::vec3( 1.5f,  0.2f, -1.5f), 
  glm::vec3(-1.3f,  1.0f, -1.5f),
};

// point light
glm::vec3 gPointLightPositions[] = {
  glm::vec3( 0.7f,  0.2f,  2.0f),
  glm::vec3( 2.3f, -3.3f, -4.0f),
  glm::vec3(-4.0f,  2.0f, -12.0f),
  glm::vec3( 0.0f,  0.0f, -3.0f),
};

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

map<unsigned int, float> key_lastFrame;

bool checkKeySensitive(unsigned int key) {
  float currentFrame = deltaTime + lastFrame;
  if (currentFrame - key_lastFrame[key] > sensitive) {
    key_lastFrame[key] = currentFrame;
    return true;
  }
  return false;
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
    if (selected_model>=0) loading_models[selected_model]->ShowBorder(false);
    if (model_count) {
      selected_model = (selected_model+1)%model_count;
      cout << "select " << selected_model << endl;
      loading_models[selected_model]->ShowBorder(true);
    }
  }

  if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && checkKeySensitive(GLFW_KEY_TAB)) {
    // <Enter>: reload shader
    initShaders();
  }
}

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
    cout << "Main::genBuffer ------" << VAO[i] << " " << VBO[i] << endl;
    glBindVertexArray(VAO[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertices), gVertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
  }

  // unbind
  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  for (int i=0; i<TEX_COUNT; i++)
    cube_texture[i] = new Texture2D(cube_texture_data[i][0], cube_texture_data[i][1]);
}

void cleanShader() {
  for (int i=0; i<SHADER_NUM; i++) {
    if (shader[i]) delete shader[i];
    shader[i] = nullptr;
  }
}

void initShaders() {
  cleanShader();

  // shader source -> shader object -> shader program
  shader[IDX_CUBE] = new Shader("vertex_specular.shader", "fragment_multiple_lights.shader");
  shader[IDX_MODEL] = new Shader("vertex_specular.shader", "fragment_model_in_lights.shader");
  shader[IDX_LAMP] = new Shader("vertex_lighted.shader", "fragment_lamp.shader");

  shader[IDX_BEHIND_CUBE] = new Shader("vertex_specular.shader", "fragment_color.shader");
  shader[IDX_BEHIND_CUBE]->setVec4("color", vec4(0.04f, 0.28f, 0.26f, 1.0f));
  
  shader[IDX_OUTLINE_MODEL] = new Shader("vertex_model_outline.shader", "fragment_outline_color.shader");
  shader[IDX_OUTLINE_MODEL]->setVec4("color", vec4(1.0f, 1.0f, 1.0f, 1.0f));

  Model::borderShader = shader[IDX_OUTLINE_MODEL];

  // direct light
  vec3 ambientLight(0.f, 0.f, 0.f);
  vec3 diffuseLight(0.1f, 0.1f, 0.1f);
  vec3 specularLight(1.0f, 1.0f, 1.0f);
  vec3 directLight(0.0f, -1.0f, 0.0f);

  for (int i : OBJ_IDXS) {
    shader[i]->setVec3("directLight.ambient", glm::value_ptr(ambientLight));
    shader[i]->setVec3("directLight.diffuse", glm::value_ptr(diffuseLight));
    shader[i]->setVec3("directLight.specular", glm::value_ptr(specularLight));

    shader[i]->setVec3("directLight.direction", glm::value_ptr(directLight));
  }

  // spot light
  ambientLight = vec3(0.0f, 0.0f, 0.0f);
  diffuseLight = vec3(0.8f, 0.8f, 0.8f);
  specularLight = vec3(1.0f, 1.0f, 1.0f);

  for (int i: OBJ_IDXS) {
    shader[i]->setVec3("spotLight.ambient", glm::value_ptr(ambientLight));
    shader[i]->setVec3("spotLight.diffuse", glm::value_ptr(diffuseLight));
    shader[i]->setVec3("spotLight.specular", glm::value_ptr(specularLight));

    shader[i]->setFloat("spotLight.constant", 1.0f);
    shader[i]->setFloat("spotLight.linear", 0.022f);
    shader[i]->setFloat("spotLight.quadratic", 0.0019);

    shader[i]->setFloat("spotLight.cutoff", glm::cos(glm::radians(15.0f)));
    shader[i]->setFloat("spotLight.cutoff_outter", glm::cos(glm::radians(20.0f)));
  }
  
  for (int i=0; i<(sizeof(gPointLightPositions)/sizeof(glm::vec3)); i++) {
    for (int j : OBJ_IDXS) {
      shader[j]->setVec3("pointLights.ambient", glm::value_ptr(ambientLight), i);
      shader[j]->setVec3("pointLights.diffuse", glm::value_ptr(diffuseLight), i);
      shader[j]->setVec3("pointLights.specular", glm::value_ptr(specularLight), i);
      
      shader[j]->setFloat("pointLights.constant", 1.0f, i);
      shader[j]->setFloat("pointLights.linear", 0.14, i);
      shader[j]->setFloat("pointLights.quadratic", 0.07f, i);

      shader[j]->setVec3("pointLights.position", glm::value_ptr(gPointLightPositions[i]), i);
    }
  }

  for (int i=0; i<TEX_COUNT; i++)
    shader[IDX_CUBE]->setInt(cube_texture[i]->uniform_name, i);

  for (int i: OBJ_IDXS)
    shader[i]->setFloat("material.shininess", 32.0f);
}

void cleanModels() {
  for (int i=0; i<model_count; i++) {
    if(loading_models[i]) delete loading_models[i];
    loading_models[i] = nullptr;
  }
}

void initModels(int n, char *path[]) {
  cleanModels();

  model_count = n;
  if (!model_count) return;

  loading_models = new Model*[model_count];
  for (int i=0; i<model_count; i++)
    loading_models[i] = new Model(path[i], flips[path[i]]);
}

int main(int argc, char *argv[]) {
  glfwInit();
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // two lines tell the version of OpenGL is 3.3
  // 用 Lab/Test/version.cc 工具查看OpenGL版本

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // will use core-profile
  // 我们只会用到OpenGL的子集，无需向后兼容的特性

#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   // open in Mac OS X
#endif
#ifdef FULL_SCREEN
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  glfwWindowHint(GLFW_RED_BITS, mode->redBits);
  glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
  glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
  glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

  WIN_WIDTH = mode->width;
  WIN_HEIGHT = mode->height;

  GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, __FILE__, monitor, NULL);
#else
  GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, __FILE__, NULL, NULL);
#endif
  if (window == NULL)
  {
      cout << "Failed to create GLFW window" << endl;
      glfwTerminate();
      return -1;
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
    return -1;
  }

  camera = new Camera::Camera(0.0f);

  initCubeData();
  initShaders();
  initModels(argc-1, argv+1);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
  glStencilMask(0x00); // 启用模板缓冲写入

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

    // 相机的信息, 作用于所有shader
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera->FieldOfView), float(WIN_WIDTH)/glm::max((float)WIN_HEIGHT,0.01f), 0.1f, 1000.0f);
    glm::mat4 view = camera->GetViewMatrix();
    for (int i=0; i<SHADER_NUM; i++) {
      shader[i]->setMatrix4("projection", glm::value_ptr(projection));
      shader[i]->setMatrix4("view", glm::value_ptr(view));
    }

/*
    // 旋转光
    float radius = 3.0f;
    float lampX = cos(glfwGetTime()*0.5f) * radius;
    float lampY = sin(glfwGetTime()*0.5f) * radius;
    glm::vec3 lightPos(lampX, lampY, 0.0f);
    glm::vec3 lightDir = glm::normalize(glm::vec3(0.0f) - lightPos);
    shader[IDX_CUBE]->setVec3("light.position", glm::value_ptr(lightPos));
    shader[IDX_CUBE]->setVec3("light.direction", glm::value_ptr(lightDir));
*/

/*
    // 头顶探照灯
    glm::vec3 lightPos = camera->Position + camera->Up;
    glm::vec3 lightDir = glm::normalize(camera->Front * 5.0f - camera->Up);
    shader[IDX_CUBE]->setVec3("light.position", glm::value_ptr(lightPos));
    shader[IDX_CUBE]->setVec3("light.direction", glm::value_ptr(lightDir));
*/
    glm::vec3 lightPos = camera->Position;  // shader的位置
    glm::vec3 lightDir = camera->Front; //glm::normalize(camera->Position + camera->Front * 200.0f - lightPos);
    for (int i : OBJ_IDXS) {
      shader[i]->setVec3("spotLight.position", glm::value_ptr(lightPos));
      shader[i]->setVec3("spotLight.direction", glm::value_ptr(lightDir));
      shader[i]->setVec3("viewPos", glm::value_ptr(camera->Position));
    }
    
    //////////////////////////////// render loading_model
    for (int i=0; i<model_count; i++)
    {
      shader[IDX_MODEL]->use();

      float radius = 10.0f;
      float lampX = sin(glm::radians(10.0f*i)) * radius;
      float lampZ = cos(glm::radians(10.0f*i)) * radius;
      float scaleV = 0.2f;
      
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(-lampX, 0, -lampZ)); // translate it down so it's at the center of the scen
      model = glm::scale(model, glm::vec3(scaleV));	// it's a bit too big for our scene, so scale it down
      shader[IDX_MODEL]->setMatrix4("model", glm::value_ptr(model));
      loading_models[i]->Draw(shader[IDX_MODEL], &model);
      Texture2D::reset();                  // use 完之后记得重置
    }

    //////////////////////////////// render CUBE
    {
      glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
      glStencilMask(0xFF); // 开启模板缓冲的写入

      shader[IDX_CUBE]->use();
      for (int i=0; i<TEX_COUNT; i++)
        cube_texture[i]->use();                // 创建了 texture 但是忘记 use，就看不到高光效果了

      glBindVertexArray(VAO[IDX_CUBE]);
      for (int i=0; i<(sizeof(gCubePositions)/sizeof(glm::vec3)); i++) {
        glm::vec3 pos = gCubePositions[i];
        glm::mat4 model(1.0f);
        model = glm::translate(model, pos);
        model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 1.0f, 0.0f));
        shader[IDX_CUBE]->setMatrix4("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      Texture2D::reset();                   // use 完之后记得重置
      glStencilMask(0x00); // 禁止模板缓冲的写入
    }
    
    //////////////////////////////// render OUTLINE
    {
      glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
      // glDepthMask(GL_FALSE);
      // glDisable(GL_DEPTH_TEST);

      shader[IDX_BEHIND_CUBE]->use();
      glBindVertexArray(VAO[IDX_BEHIND_CUBE]);
      for (int i=0; i<(sizeof(gCubePositions)/sizeof(glm::vec3)); i++) {
        glm::vec3 pos = gCubePositions[i];
        glm::mat4 model(1.0f);
        model = glm::translate(model, pos);
        model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(1.1f));
        shader[IDX_BEHIND_CUBE]->setMatrix4("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      Texture2D::reset();                   // use 完之后记得重置

      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      // glEnable(GL_DEPTH_TEST);  // 后面的继续做深度测试
      // glDepthMask(GL_TRUE);
    }
 /*   
    //////////////////////////////// render CUBE & OUTLINE
    {
      glBindVertexArray(VAO[IDX_CUBE]);
      
      for (int i=0; i<(sizeof(gCubePositions)/sizeof(glm::vec3)); i++) {
        // 填充模板缓冲区(绘制物体的片段全部为1)
#ifdef OUTLINE_DEPTH
        glClear(GL_STENCIL_BUFFER_BIT);
#endif

        shader[IDX_CUBE]->use();
        for (int i=0; i<TEX_COUNT; i++)
          cube_texture[i]->use();                // 创建了 texture 但是忘记 use，就看不到高光效果了

        glm::vec3 pos = gCubePositions[i];
        glm::mat4 model(1.0f);
        model = glm::translate(model, pos);
        model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 1.0f, 0.0f));
        shader[IDX_CUBE]->setMatrix4("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glStencilOp(GL_KEEP, GL_ZERO, GL_REPLACE);
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);      // 缓冲区取反
#ifndef OUTLINE_DEPTH
        glDisable(GL_DEPTH_TEST);
#endif 

        shader[IDX_BEHIND_CUBE]->use();
        shader[IDX_BEHIND_CUBE]->setMatrix4("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
#ifndef OUTLINE_DEPTH
        glEnable(GL_DEPTH_TEST);
#endif 
      }

      Texture2D::reset();                   // use 完之后记得重置
    }
*/
    //////////////////////////////// render point light
    {
      shader[IDX_LAMP]->use();
      glBindVertexArray(VAO[IDX_CUBE]);

      for (int i=0; i<(sizeof(gPointLightPositions)/sizeof(glm::vec3)); i++) {
        glm::vec3 pos = gPointLightPositions[i];
        glm::mat4 model(1.0f);
        model = glm::translate(model, pos);
        // model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 0.5f, 1.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        shader[IDX_LAMP]->setMatrix4("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }

    //////////////////////////////// render spotlight 手电筒
    {
      shader[IDX_LAMP]->use();
      glBindVertexArray(VAO[IDX_CUBE]);

      // 右手持
      lightPos += camera->Right + camera->Front * 3.0f; // 绘制光源的位置, 会有一点出入. 目的是保证手电筒的光锥是直的.
      glm::mat4 model(1.0f);
      model = glm::translate(model, lightPos);
      model = glm::scale(model, glm::vec3(0.2f));
      shader[IDX_LAMP]->setMatrix4("model", glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

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