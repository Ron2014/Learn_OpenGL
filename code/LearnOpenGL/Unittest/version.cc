#include "UnitTest++/src/UnitTest++.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define DEFAULT_SCR_W 800
#define DEFAULT_SCR_H 600

GLFWwindow* window = nullptr;

using namespace std;

TEST(Version) {
	int argc = 1;
	char *argv[] = {__FILE__, NULL};
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

	window = glfwCreateWindow(DEFAULT_SCR_W, DEFAULT_SCR_H, __FILE__, NULL, NULL);

	if (!window){
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);         // 当前线程与window绑定

	// walk around
	// look around
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// zoom in/out
	// glad 必须在 windows 创建成功后初始化
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		glfwTerminate();
		exit(-1);
	}

 
	const GLubyte* name = glGetString(GL_VENDOR);           //返回负责当前OpenGL实现厂商的名字
	const GLubyte* biaoshifu = glGetString(GL_RENDERER);    //返回一个渲染器标识符，通常是个硬件平台
	const GLubyte* OpenGLVersion = glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
  /*
	printf("OpenGL实现厂商的名字：%s\n", name);
	printf("渲染器标识符：%s\n", biaoshifu);
	printf("OpenGL实现的版本号：%s\n", OpenGLVersion);
	printf("OGLU工具库版本：%s\n", gluVersion);
  */
   
    printf("OpenGL Manufacturer: %s\n", name);
	printf("Rendering Identity: %s\n", biaoshifu);
	printf("OpenGL Version: %s\n", OpenGLVersion);
    
	//如果是在VS上执行，需要在return前加上：system("pause");

  	glfwTerminate();
}