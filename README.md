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

所以，GLFW解决的是操作系统的适配，GLAD解决的是显卡（驱动）的适配。

## 什么是 GLUT

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