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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  cout << "veiw change " << width << " " << height << endl;
  glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);           // response to ESC
  else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if ((int)glfwGetTime()%2)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

bool checkError(const int objID, const int process, const char *msg) {
  int success;
  char info[INFO_LEN];
  glGetShaderiv(objID, process, &success);
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
  if (checkError(objID, GL_COMPILE_STATUS, errMsg)) {
    shaders.push_back(objID);
  }
}

int createProgram(vector<int> &shaders) {
  int shaderProgram = glCreateProgram();
  for (auto objID : shaders) {
    glAttachShader(shaderProgram, objID);
  }
  glLinkProgram(shaderProgram);

  if (checkError(shaderProgram, GL_LINK_STATUS, "ERROR: Shader Program LINKING FAILED")) {
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

#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   // open in Mac OS X
#endif

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

  float vertices[] = {
     0.5f,  0.5f, 0.0f,   // 右上角
     0.5f, -0.5f, 0.0f,   // 右下角
    -0.5f, -0.5f, 0.0f,   // 左下角
    -0.5f,  0.5f, 0.0f    // 左上角
  };

  unsigned int indices[] = { // 注意索引从0开始! 
      0, 1, 3, // 第一个三角形
      1, 2, 3  // 第二个三角形
  };

  unsigned int VBO[BUF_LEN], VAO[BUF_LEN], EBO[BUF_LEN];
  glGenVertexArrays(BUF_LEN, VAO);      // 第二个参数实际上表示一个数组, 第一个参数表示数组大小.
  glGenBuffers(BUF_LEN, VBO);           // 所以如果是一个 unsigned int 得用&, 如 glGenVertexArrays(1, &VBO);
  glGenBuffers(BUF_LEN, EBO);

  // 将顶点数据存在一个缓冲区中怎么做?
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); //  全都存到 VBO[0] 中, VBO[1] 是空闲的
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // 但是 两个 VAO 保存不同的顶点属性. 也就是说, 用VAO区分不同的三角形
  // VAO 保存的是顶点属性调用 glVertexAttribPointer/glEnableVertexAttribArray/glDisableVertexAttribArray
  // 暂且理解成调用这些接口的参数缓存在 VAO 中.

  // rect 0
  glBindVertexArray(VAO[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  /**
   * 1. 顶点属性编号
   * 2. 包含3个变量
   * 3. 每个变量类型为float
   * 4. 是否标准化. 我们已经保证了向量分量都在[-1.0, 1.0]之间, 所以不需要标准化.
   * 5. 共占用内存长度(步长)
   * 6. 在缓冲区中的起始位置
  */
  glEnableVertexAttribArray(0);

  // unbind
  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

    // draw our rect by 6 vertex
    glUseProgram(shaderProgram);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBindVertexArray(VAO[0]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);              // double buffer switch
    glfwPollEvents();                     // keyboard/mouse event
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(BUF_LEN, VAO);
  glDeleteBuffers(BUF_LEN, VBO);
  glDeleteBuffers(BUF_LEN, EBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}