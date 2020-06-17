#include <iostream>
using namespace std;

#include "model.h"
#include "shader.h"
#include "texture2d.h"
#include "camera.h"

#include <vector>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define BUFF_LEN 3

enum {
  IDX_LAMP,
  IDX_CUBE,
  IDX_OUTLINE,
  IDX_MODEL,
  SHADER_NUM,
};

int OBJ_IDXS[] = {IDX_CUBE, IDX_MODEL};

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

// 不知为何 backpack的贴图没有Y方向颠倒, 这省去了flip操作.
map<string, bool> flips = { {"nanosuit", true}, {"backpack", false} };

unsigned int WIN_WIDTH = 800;
unsigned int WIN_HEIGHT = 600;

Shader *shader[SHADER_NUM];
Camera::Camera *camera;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX=WIN_WIDTH*0.5f, lastY=WIN_HEIGHT*0.5f;
bool firstMouse = true;

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

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);           // response to ESC

  if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    if ((int)glfwGetTime()%2)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   // open in Mac OS X
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

  // shader source -> shader object -> shader program
  shader[IDX_LAMP] = new Shader("vertex_lighted.shader", "fragment_lamp.shader");
  shader[IDX_CUBE] = new Shader("vertex_specular.shader", "fragment_multiple_lights.shader");
  shader[IDX_OUTLINE] = new Shader("vertex_back_facing.shader", "fragment_color.shader");
  // shader[IDX_OUTLINE] = new Shader("vertex_specular.shader", "fragment_color.shader");
  shader[IDX_MODEL] = new Shader("vertex_specular.shader", "fragment_model_in_lights.shader");

  shader[IDX_OUTLINE]->setVec4("color", vec4(0.04f, 0.28f, 0.26f, 1.0f));

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

  // point light
  glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f),
  };
  
  for (int i=0; i<(sizeof(pointLightPositions)/sizeof(glm::vec3)); i++) {
    for (int j : OBJ_IDXS) {
      shader[j]->setVec3("pointLights.ambient", glm::value_ptr(ambientLight), i);
      shader[j]->setVec3("pointLights.diffuse", glm::value_ptr(diffuseLight), i);
      shader[j]->setVec3("pointLights.specular", glm::value_ptr(specularLight), i);
      
      shader[j]->setFloat("pointLights.constant", 1.0f, i);
      shader[j]->setFloat("pointLights.linear", 0.14, i);
      shader[j]->setFloat("pointLights.quadratic", 0.07f, i);

      shader[j]->setVec3("pointLights.position", glm::value_ptr(pointLightPositions[i]), i);
    }
  }

  Texture2D *cube_texture[TEX_COUNT];
  for (int i=0; i<TEX_COUNT; i++) {
    cube_texture[i] = new Texture2D(cube_texture_data[i][0], cube_texture_data[i][1]);
    shader[IDX_CUBE]->setInt(cube_texture[i]->uniform_name, i);
  }

  for (int i: OBJ_IDXS)
    shader[i]->setFloat("material.shininess", 32.0f);

  camera = new Camera::Camera(0.0f);

  float vertices[] = {
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

  glm::vec3 cubePositions[] = {
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

  int model_count = argc-1;
  Model **loading_models = nullptr;
  if (model_count) {
    loading_models = new Model*[model_count];
    for (int i=0; i<model_count; i++)
      loading_models[i] = new Model(argv[i+1], flips[argv[i+1]]);
  }
  

  unsigned int VBO[BUFF_LEN], VAO[BUFF_LEN];
  glGenVertexArrays(BUFF_LEN, VAO);      // 第二个参数实际上表示一个数组, 第一个参数表示数组大小.
  glGenBuffers(BUFF_LEN, VBO);           // 所以如果是一个 unsigned int 得用&, 如 glGenVertexArrays(1, &VBO);

  for (int i=0; i<BUFF_LEN; i++) {
    cout << "Main::genBuffer ------" << VAO[i] << " " << VBO[i] << endl;
    glBindVertexArray(VAO[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture cooridnate
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
  }

  // unbind
  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0); 
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);

  // 填充模板缓冲区(绘制物体的片段全部为1)
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glStencilMask(0xFF); // 启用模板缓冲写入
  glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
  
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
    // 右手持手电筒
    glm::vec3 lightPos = camera->Position;  // shader的位置
    glm::vec3 lightDir = camera->Front; //glm::normalize(camera->Position + camera->Front * 200.0f - lightPos);
    for (int i : OBJ_IDXS) {
      shader[i]->setVec3("spotLight.position", glm::value_ptr(lightPos));
      shader[i]->setVec3("spotLight.direction", glm::value_ptr(lightDir));
    }
    lightPos += camera->Right + camera->Front * 3.0f; // 绘制光源的位置, 会有一点出入. 目的是保证手电筒的光锥是直的.

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera->FieldOfView), float(WIN_WIDTH)/glm::max((float)WIN_HEIGHT,0.01f), 0.1f, 1000.0f);
    glm::mat4 view = camera->GetViewMatrix();

    for (int i : OBJ_IDXS) shader[i]->setVec3("viewPos", glm::value_ptr(camera->Position));

    for (int i=0; i<SHADER_NUM; i++) {
      shader[i]->setMatrix4("projection", glm::value_ptr(projection));
      shader[i]->setMatrix4("view", glm::value_ptr(view));
    }
    
    /**
     * 我们在上方(或上一帧)开启了模板测试方式GL_ALWAYS
     * 理论上, 下面模型渲染的片段区域会填充为1
     * 但这并不会对逻辑造成影响
     * 
     * 因为我们描边使用的使用的是箱子的顶点位置和法线
     * 而且是通过 GL_NOTEQUAL 1 的方式
     * 天然避开了模型的片段
    */
    //////////////////////////////// render loading_model
    for (int i=0; i<model_count; i++)
    {
      shader[IDX_MODEL]->use();

      glm::mat4 model = glm::mat4(1.0f);
      float radius = 10.0f;
      float lampX = sin(glm::radians(10.0f*i)) * radius;
      float lampZ = cos(glm::radians(10.0f*i)) * radius;
      model = glm::translate(model, glm::vec3(-lampX, -5.0f, -lampZ)); // translate it down so it's at the center of the scene
      model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
      shader[IDX_MODEL]->setMatrix4("model", glm::value_ptr(model));

      loading_models[i]->Draw(shader[IDX_MODEL]);
      Texture2D::reset();                  // use 完之后记得重置
    }

    //////////////////////////////// render CUBE
    {
      shader[IDX_CUBE]->use();
      for (int i=0; i<TEX_COUNT; i++)
        cube_texture[i]->use();                // 创建了 texture 但是忘记 use，就看不到高光效果了

      glBindVertexArray(VAO[IDX_CUBE]);
      for (int i=0; i<(sizeof(cubePositions)/sizeof(glm::vec3)); i++) {
        glm::vec3 pos = cubePositions[i];
        glm::mat4 model(1.0f);
        model = glm::translate(model, pos);
        model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 1.0f, 0.0f));
        shader[IDX_CUBE]->setMatrix4("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      Texture2D::reset();                   // use 完之后记得重置
    }
    
    //////////////////////////////// render OUTLINE
    {
      /**
       * 放大物体为原来1.1倍
       * 不等于1的部分(非物体部分才会测试通过)
       *
       * 缩放的方式并不能保证表面被覆盖, 因为每个顶点在设计时(模型文件中)都已经存在了偏移位置(顶点坐标)
       * 缩放会导致偏移也会缩放. 而且缩放影子对于不同大小的模型, 会对应不同的描边厚度.
       * 正确的做法是将顶点坐标和法线方向做偏移, 就能将描边厚度具体的值(偏移的模).
       * 
       * 关掉Z缓冲(深度测试)的结果是:
       * 描边的片段不会去做深度测试, 更不会修改深度缓冲区的值.
       * 
       * 这就导致有非描边的片段可以(ALWAYS通过所有模板缓冲且按照深度测试)正常绘制
       * 也就是说: 描边片段仅按照模板缓冲绘制(因为此时深度测试关掉了), 非描边片段仅按照深度测试绘制(因为此时模板缓冲全部通过)
       * 
       * 最终效果就是: 描边像一层光圈一样覆盖在物体表面, 但是这个光本身不会和任何颜色混合, 它会优先被颜色片段覆盖.
       */
      glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
      /**
       * 为什么要禁止模板写入呢?
       * 测试通过时写入, 会增加模板缓冲区1的范围, 描边的区域也添加进去了
       * 这会增加后续 NOTEQUAL 1 的命中率, 过滤掉多余的计算(先模板测试, 才会有深度测试)
       * 因为 NOTEQUAL 1 仅对描边操作(后面会解释对于非描边片段会打开GL_NOTEQUAL)
       * 
       * 这表示当遇到非描边片段和已经绘制的描边片段, 我们都不需要再考虑描边了:
       * 前者有优先渲染权(覆盖), 后者也不需要考虑(描边颜色是单色, 没有颜色混合的操作)
       * 而这个片段范围, 刚好可以被[开启模板缓冲写入的模板缓冲区]所描述.
       * 所以此处根本没有禁用写入的需要哇!
       */
      // glStencilMask(0x00); // 禁止模板缓冲的写入
      glDisable(GL_DEPTH_TEST);

      shader[IDX_OUTLINE]->use();
      glBindVertexArray(VAO[IDX_OUTLINE]);
      for (int i=0; i<(sizeof(cubePositions)/sizeof(glm::vec3)); i++) {
        glm::vec3 pos = cubePositions[i];
        glm::mat4 model(1.0f);
        model = glm::translate(model, pos);
        model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(1.1f));
        shader[IDX_OUTLINE]->setMatrix4("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      Texture2D::reset();                   // use 完之后记得重置
      
      /**
       * 不加这一句, 上面的glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
       * 依然奏效, 模板缓冲区在下一帧刷新(全为0)之后, 渲染模型时, 只会渲染模板值为[0, 1.0)的片段
       * 模板值为1.0的片段会被丢弃(看上去像是挖去了一些顶点)
       * 除非在清理缓冲之后, 立即指定 glStencilFunc(GL_ALWAYS, 1, 0xFF); 让所有的片段都应该更新模板缓冲
       * 但是这依然有问题, 如果模型渲染在这一帧的后面, 还是会因为模板测试过滤掉.
       * 所以此处必须将ALWAYS打开
       */
      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      // glStencilMask(0xFF);
      glEnable(GL_DEPTH_TEST);  // 后面的继续做深度测试
    }

/*    
    //////////////////////////////// render loading_model
    for (int i=0; i<model_count; i++)
    {
      shader[IDX_MODEL]->use();

      glm::mat4 model = glm::mat4(1.0f);
      float radius = 10.0f;
      float lampX = sin(glm::radians(10.0f*i)) * radius;
      float lampZ = cos(glm::radians(10.0f*i)) * radius;
      model = glm::translate(model, glm::vec3(-lampX, -5.0f, -lampZ)); // translate it down so it's at the center of the scene
      model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
      shader[IDX_MODEL]->setMatrix4("model", glm::value_ptr(model));

      loading_models[i]->Draw(shader[IDX_MODEL]);
      Texture2D::reset();                  // use 完之后记得重置
    }
*/
    //////////////////////////////// render point light
    {
      shader[IDX_LAMP]->use();
      glBindVertexArray(VAO[IDX_LAMP]);

      for (int i=0; i<(sizeof(pointLightPositions)/sizeof(glm::vec3)); i++) {
        glm::vec3 pos = pointLightPositions[i];
        glm::mat4 model(1.0f);
        model = glm::translate(model, pos);
        // model = glm::rotate(model, (float)glfwGetTime()+20.0f*(i+1), glm::vec3(0.5f, 0.5f, 1.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        shader[IDX_LAMP]->setMatrix4("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    }

    //////////////////////////////// render spotlight
    {
      shader[IDX_LAMP]->use();
      glBindVertexArray(VAO[IDX_LAMP]);

      glm::mat4 model(1.0f);
      model = glm::translate(model, lightPos);
      model = glm::scale(model, glm::vec3(0.2f));
      shader[IDX_LAMP]->setMatrix4("model", glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);              // double buffer switch
    glfwPollEvents();                     // keyboard/mouse event
  }
  
  for (int i=0; i<SHADER_NUM; i++){
    cout << "Main::cleanBuffer ------" << VAO[i] << " " << VBO[i] << endl;
    delete shader[i];
  }
  for (int i=0; i<TEX_COUNT; i++)
    delete cube_texture[i];
  for (int i=0; i<model_count; i++)
    delete loading_models[i];
  delete camera;

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(BUFF_LEN, VAO);
  glDeleteBuffers(BUFF_LEN, VBO);

  glfwTerminate();
  return 0;
}