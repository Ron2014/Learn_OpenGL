#include <iostream>
using namespace std;

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define INFO_LEN 512

const unsigned int WIN_WIDTH = 800;
const unsigned int WIN_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
  "layout (location=0) in vec3 aPos;\n"
  "layout (location=1) in vec3 aColor;\n"
  "out vec3 ourColor;\n"
  "void main() {\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "   ourColor = aColor;\n"
  "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
  "in vec3 ourColor;\n"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "   FragColor = vec4(ourColor, 1.0f);"
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

  GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Shaders", NULL, NULL);
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
    // position           // color
    -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,     // left-bottom is red
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,     // right-bottom is green
     0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,     // top is blue
  };

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 顶点属性0 layout (location = 0)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // 顶点属性1 layout (location = 1) 注意起始位置要越过前面表示位置的3个分量float
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  // unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0); 


  // render loop:
  while(!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);

    /**
     * 这是在片段着色器中进行的所谓片段插值(Fragment Interpolation)的结果。
     * 当渲染一个三角形时，光栅化(Rasterization)阶段通常会造成比原指定顶点更多的片段。
     * 光栅会根据每个片段在三角形形状上所处相对位置决定这些片段的位置
     * 基于这些位置，它会插值(Interpolate)所有片段着色器的输入变量。
     * 比如说，我们有一个线段，上面的端点是绿色的，下面的端点是蓝色的。
     * 如果一个片段着色器在线段的70%的位置运行，它的颜色输入属性就会是一个绿色和蓝色的线性结合；更精确地说就是30%蓝 + 70%绿。
     * 
     * 我们仅指定了3个顶点的颜色(注意这3个颜色是不同的).
     * 光栅化替我们完成了这3个颜色在片段中的过度.
     * 这个过度的过程: 线性插值
    */

    glfwSwapBuffers(window);              // double buffer switch
    glfwPollEvents();                     // keyboard/mouse event
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}