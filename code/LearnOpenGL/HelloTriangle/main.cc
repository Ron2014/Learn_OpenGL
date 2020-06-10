#include <iostream>
using namespace std;

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define INFO_LEN 512
#define BUF_LEN 3

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
  "layout (location=0) in vec3 aPos;\n"
  "void main() {\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);"
  "}\0";

const char *vertexShaderSource1 = "#version 330 core\n"
  "layout (location=1) in vec3 aPos;\n"             // 顶点属性定义在1的位置
  "void main() {\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

const char *fragmentShaderSource1 = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "   FragColor = vec4(0.5f, 0.0f, 1.0f, 0.0f);"    // 半透明, 这个倒三角会覆盖在上面
  "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  cout << "veiw change " << width << " " << height << endl;
  glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);           // response to ESC
}

bool checkError(const int objID, const char *msg) {
  int success;
  char info[INFO_LEN];
  glGetShaderiv(objID, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(objID, INFO_LEN, NULL, info);
    printf("%s:\n", msg);
    printf("%s\n", info);
    return false;
  }
  return true;
}

void createShader(GLenum shader_type, const char* shader_source, vector<int> &shaders) {
  int objID = glCreateShader(shader_type);
  glShaderSource(objID, 1, &shader_source, NULL);
  glCompileShader(objID);

  char errMsg[INFO_LEN];
  sprintf(errMsg, "ERROR: Shader %d COMPILATION FAILED", shader_type);
  if (checkError(objID, errMsg)) {
    shaders.push_back(objID);
  }
}

int createProgram(vector<int> &shaders) {
  int shaderProgram = glCreateProgram();
  for (auto objID : shaders) {
    glAttachShader(shaderProgram, objID);
  }
  glLinkProgram(shaderProgram);

  if (checkError(shaderProgram, "ERROR: Shader Program LINKING FAILED")) {
    for (auto objID : shaders) {
      glDeleteShader(objID);
    }
  }
  return shaderProgram;
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

  GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Hello Triangle", NULL, NULL);
  if (window == NULL)
  {
      cout << "Failed to create GLFW window" << endl;
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(window);         // 当前线程与window绑定
  // glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);             // glad API，设置渲染窗口的尺寸
  glfwSetWindowSizeCallback(window, framebuffer_size_callback);

  // glad 必须在 windows 创建成功后初始化
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Failed to initialize GLAD" << endl;
    return -1;
  }

  // shader source -> shader object -> shader program
  vector<int> shaders;
  createShader(GL_VERTEX_SHADER, vertexShaderSource, shaders);
  createShader(GL_FRAGMENT_SHADER, fragmentShaderSource, shaders);
  int shaderProgram = createProgram(shaders);

  shaders.clear();
  createShader(GL_VERTEX_SHADER, vertexShaderSource1, shaders);
  createShader(GL_FRAGMENT_SHADER, fragmentShaderSource1, shaders);
  int shaderProgram1 = createProgram(shaders);

  float vertices[] = {
    // triangle 0
    -0.5f, -0.5f, 0.0f,   // bottom-left
     0.5f, -0.5f, 0.0f,   // bottom-right
     0.0f,  0.5f, 0.0f,   // top-middle

    // triangle 1
    -0.5f,  0.5f, 0.0f,   // top-left
     0.5f,  0.5f, 0.0f,   // top-right
     0.0f, -0.5f, 0.0f,   // bottom-middle
  };

  cout << sizeof(vertices) << endl;

  unsigned int VBO[BUF_LEN], VAO[BUF_LEN];
  glGenVertexArrays(BUF_LEN, VAO);      // 第二个参数实际上表示一个数组, 第一个参数表示数组大小.
  glGenBuffers(BUF_LEN, VBO);           // 所以如果是一个 unsigned int 得用&, 如 glGenVertexArrays(1, &VBO);

/**
  // 下面的例子, 将 vertices (内存数据) 存到显存的两个缓冲区域中
  // 平时也许不会这么干.

  // triangle 0
  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) / 2, vertices, GL_STATIC_DRAW);

  // 注意 (void*)0, 表示的是数据在缓冲区的起始位置(地址偏移)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // triangle 1
  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) / 2, vertices + 9, GL_STATIC_DRAW);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
*/

  // 将顶点数据存在一个缓冲区中怎么做?
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); //  全都存到 VBO[0] 中, VBO[1] 是空闲的
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 但是 两个 VAO 保存不同的顶点属性. 也就是说, 用VAO区分不同的三角形
  // VAO 保存的是顶点属性调用 glVertexAttribPointer/glEnableVertexAttribArray/glDisableVertexAttribArray
  // 暂且理解成调用这些接口的参数缓存在 VAO 中.

  // triangle 0
  glBindVertexArray(VAO[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // triangle 1
  glBindVertexArray(VAO[1]);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(9*sizeof(float)));
  glEnableVertexAttribArray(1);


  // unbind
  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0); 


  // render loop:
  while(!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our triangle 0
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO[0]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // draw our triangle 1
    glUseProgram(shaderProgram1);
    glBindVertexArray(VAO[1]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // 拿出不同的喷枪, 将三角形按照各自的着色器糊到墙上.
    // 可以看到修改第二个三角形的alpha值并不会将两个三角形的颜色混合....
    // 这个问题留给以后的学习.

    glfwSwapBuffers(window);              // double buffer switch
    glfwPollEvents();                     // keyboard/mouse event
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(BUF_LEN, VAO);
  glDeleteBuffers(BUF_LEN, VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}