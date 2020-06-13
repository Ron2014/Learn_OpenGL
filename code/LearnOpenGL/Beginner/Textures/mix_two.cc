#include <iostream>
using namespace std;

#include "shader.h"
#include "texture2d.h"

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define BUF_LEN 1

unsigned int WIN_WIDTH = 800;
unsigned int WIN_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  cout << "veiw change " << width << " " << height << endl;
  glViewport(0, 0, width, height);
  WIN_WIDTH = width;
  WIN_HEIGHT = height;
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

  GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Textures", NULL, NULL);
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
  Shader shader("vertex_texture.shader", "fragment_mix_texture.shader");
  Texture2D texture0("container.jpg");
  Texture2D texture1("awesomeface.png");

  float vertices[] = {
//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 图片的右上, 钉在屏幕第一象限的中心. 红色
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 图片的右下, 钉在屏幕第四象限的中心. 绿色
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 图片的左下, 钉在屏幕第三象限的中心. 蓝色
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 图片的左上, 钉在屏幕第二象限的中心. 黄色
  };

  unsigned int indices[] = { // 注意索引从0开始! 
      0, 1, 3, // 第一个三角形
      1, 2, 3  // 第二个三角形
  };

  unsigned int VBO[BUF_LEN], VAO[BUF_LEN], EBO[BUF_LEN];
  glGenVertexArrays(BUF_LEN, VAO);      // 第二个参数实际上表示一个数组, 第一个参数表示数组大小.
  glGenBuffers(BUF_LEN, VBO);           // 所以如果是一个 unsigned int 得用&, 如 glGenVertexArrays(1, &VBO);
  glGenBuffers(BUF_LEN, EBO);

  // rect 0
  glBindVertexArray(VAO[0]);

  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); //  全都存到 VBO[0] 中, VBO[1] 是空闲的
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // 但是 两个 VAO 保存不同的顶点属性. 也就是说, 用VAO区分不同的三角形
  // VAO 保存的是顶点属性调用 glVertexAttribPointer/glEnableVertexAttribArray/glDisableVertexAttribArray
  // 暂且理解成调用这些接口的参数缓存在 VAO 中.

  /**
   * 1. 顶点属性编号
   * 2. 包含3个变量
   * 3. 每个变量类型为float
   * 4. 是否标准化. 我们已经保证了向量分量都在[-1.0, 1.0]之间, 所以不需要标准化.
   * 5. 共占用内存长度(步长)
   * 6. 在缓冲区中的起始位置
  */

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coordinate
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
  glEnableVertexAttribArray(2);

  // unbind
  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0); 

  shader.setInt("texture0", 0);
  shader.setInt("texture1", 1);

  // render loop:
  while(!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our rect by 6 vertex
    texture0.use();
    texture1.use();
    
    shader.use();

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

  glfwTerminate();
  return 0;
}