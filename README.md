## 什么是 GLFW 库

在我们画出出色的效果之前，首先要做的就是创建一个OpenGL上下文(Context)和一个用于显示的窗口。然而，这些操作在每个系统上都是不一样的，OpenGL有目的地从这些操作抽象(Abstract)出去。这意味着我们不得不自己处理创建窗口，定义OpenGL上下文以及处理用户输入。

所谓抽象，就是OpenGL没有实现的部分。很多针对操作系统的API，OpenGL一般都是给出函数原型的定义，但实际实现，需要接入相应的库，将这些弱符号替换掉。最流行的几个库有GLUT，SDL，SFML和GLFW。

它们用来提供简化的系统级API，保证我们我们的代码与操作系统（设备）无关。即，实现了可移植性（跨平台）。

## 什么是 GLAD 库

因为OpenGL只是一个标准/规范，具体的实现是由驱动开发商针对特定显卡实现的。由于OpenGL驱动版本众多，它大多数函数的位置都无法在编译时确定下来，需要在运行时查询。所以任务就落在了开发者身上，开发者需要在运行时获取函数地址并将其保存在一个函数指针中供以后使用。取得地址的方法因平台而异，在Windows上会是类似这样：

```c
// 定义函数原型
typedef void (*GL_GENBUFFERS) (GLsizei, GLuint*);
// 找到正确的函数并赋值给函数指针
GL_GENBUFFERS glGenBuffers  = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");
// 现在函数可以被正常调用了
GLuint buffer;
glGenBuffers(1, &buffer);
```

你可以看到代码非常复杂，而且很繁琐，我们需要对每个可能使用的函数都要重复这个过程。幸运的是，有些库能简化此过程，其中GLAD是目前最新，也是最流行的库。

所以，GLFW解决的是操作系统的适配，GLAD解决的是显卡（驱动）的适配。用上面的例子理解，GLAD具体是通过管理函数地址来工作的。

## 什么是 GLUT 库

GLUT(OpenGL Utility Toolkit) 的功能等同于 GLFW，提供了一整套与设备无关的窗口API. 也可以说 GLFW 是 GLUT-like Windowing。

只是GLUT已经20年没有维护了，OpenGL官网推荐我们使用[freegult](https://sourceforge.net/projects/freeglut/)。

关于跨平台，可以简单理解成OpenGL定义了统一的系统调用的API函数原型，具体的实现是由各大操作系统自己完成的。这就有了

- GLX, OpenGL Extension to the X Window - Unix使用
- AGL, Apple GL                         - 苹果系统
- WGL                                   - 微软的Windows系统
- PGL, Presentation Manager to OpenGL   - IBM OS/2

这些库就能提供上面所说的OpenGL上下文（执行环境）。然而对于一款跨平台的图形界面应用，各种库的接入、系统调用时不同运行环境（通常通过宏）的判断等会让程序显得非常繁琐。

为了节省开发者调试平台适配的时间，更加专注于自身软件的功能实现，就需要有个库将这些不同平台的库进行封装。

GLFW 和 GLUT 就是做这种“打包”工作的。

## 学习路线

可选路线包括:

- 看书
  1. 3D数学基础:图形与游戏开发
  2. DirectX 9.0 3D游戏开发编程基础
  3. 计算机图形学 第4版 
  4. Unity Shader入门
  5. Unity Shaders and Effects Cookbook [中文教学](https://blog.csdn.net/candycat1992/category_1782159_2.html)
  6. C++ GUI QT4编程(底层也是OpenGL)
- LearnOpenGL(配套中文版, 还有B站傅老师的`OpenGL自製3D遊戲引擎`)
- Unreal官方教学视频(渲染/光照/材质等偏向应用的部分)
- Unity官方教学视频(就看网站能不能打开了)

[LearnOpenGL官网](https://learnopengl.com/) 使用的是 GLFW + GLAD. 

《Computer Graphics with OpenGL》中使用的是 GLUT + GLU (这个GLUT多年没更新了, OpenGL官网推荐用freeglut替代).

B站傅老师则使用的是 GLFW + GLEW.

因此项目中第三方库(code/Ext)比较多。在完成了[LearnOpenGL 的 Hello Window](https://learnopengl.com/Getting-started/Hello-Window) 以及《Computer Graphics with OpenGL》第二章的第一个OpenGL程序之后，表示所有开发环境都已搭建完成。

之后，可以对照[LearnOpenGL中文网站](https://learnopengl-cn.github.io/) 的翻译逐课学习。

总之，网站学习为主，书籍（2004年出版，内容未免老旧，概念、术语不及如今丰富）学习为辅。

不过网站的内容也不算最新，OpenGL版本停留在3.3。

## 关于库

Nuget是一个.NET平台下的开源的项目，它是Visual Studio的扩展。在使用Visual Studio开发基于.NET Framework的应用时，Nuget能把在项目中添加、移除和更新引用的工作变得更加快捷方便.

NuGet程序包的方式已成为微软主推的组件安装方式，几乎各个组件都在NuGet上找得到，近到创建Web项目默认使用NuGet程序包，远到.NET Core完全使用了NuGet组件安装，NuGet已然成为了未来的趋势，相信它在未来可以做得更好。

目前为止, freeglut glfw glew 在 NuGet 上都能找到(前提先建立一个项目). 这也是通过B站傅老师视频才知道的功能.