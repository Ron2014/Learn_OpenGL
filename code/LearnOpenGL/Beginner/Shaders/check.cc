#include <iostream>
using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

  GLFWwindow* window = glfwCreateWindow(800, 600, "Shaders", NULL, NULL);
  if (window == NULL)
  {
      cout << "Failed to create GLFW window" << endl;
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(window);         // 当前线程与window绑定

  // glad 必须在 windows 创建成功后初始化
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Failed to initialize GLAD" << endl;
    return -1;
  }    

	const GLubyte* vendor = glGetString(GL_VENDOR);           // 返回负责当前OpenGL实现厂商的名字
	const GLubyte* render = glGetString(GL_RENDERER);         // 返回一个渲染器标识符，通常是个硬件平台
	const GLubyte* version = glGetString(GL_VERSION);         // 返回当前OpenGL实现的版本号
  const GLubyte* max_vertex_attribs = glGetString(GL_MAX_VERTEX_ATTRIBS);
  int nrAttribs;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttribs);         // 29个包含4分量的顶点属性
  
  printf("OpenGL Manufacturer: %s\n", vendor);
	printf("Rendering Identity: %s\n", render);
	printf("OpenGL Version: %s\n", version);
	printf("Maximum nr of vertex attributes supported: %d %s\n", nrAttribs, max_vertex_attribs);

  glfwTerminate();
  return 0;
}