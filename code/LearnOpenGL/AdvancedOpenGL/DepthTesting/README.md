## 概念

缓冲区 

1. 颜色缓冲 GL_COLOR_BUFFER_BIT. 储存所有的片段颜色：视觉输出.
2. 深度缓冲 GL_DEPTH_BUFFER_BIT. 深度缓冲(Depth Buffer) 或z缓冲(z-buffer). 它会以16、24或32位float的形式储存它的深度值。在大部分的系统中，深度缓冲的精度都是24位的。

当深度测试(Depth Testing)被启用的时候，OpenGL会将一个片段的的深度值与深度缓冲的内容进行对比. **深度缓冲的初始值为1.0, 表示无穷远.**

## 渲染管线中的流程

屏幕空间(裁剪空间)中运行.

片段着色器 -> 模板测试 -> 深度测试

屏幕空间坐标与通过OpenGL的glViewport所定义的视口密切相关，并且可以直接使用GLSL内建变量gl_FragCoord从片段着色器中直接访问。

### gl_FragCoord

**屏幕空间坐标原点 - 左下角** (非屏幕中心, 非左上角且方向向下)

- x - 横坐标
- y - 纵坐标
- z - 深度值

## 使用步骤

### 开启深度测试

默认是禁用的

glEnable(GL_DEPTH_TEST);

### 刷新缓冲区

glClear(GL_DEPTH_BUFFER_BIT);

### 设置比较函数

glDepthFunc(GLenum func);

- func 比较的操作码/条件. 如果片段的深度值与缓冲区的深度值不满足操作码的条件, 则会被丢弃.

左操作数: 片段的深度值 gl_FragCoord.z
右操作数: 深度缓冲区的值

|函数|描述|
|--|--|
GL_ALWAYS|永远通过深度测试
GL_NEVER|永远不通过深度测试
GL_LESS|在片段深度值小于缓冲的深度值时通过测试
GL_EQUAL|在片段深度值等于缓冲区的深度值时通过测试
GL_LEQUAL|在片段深度值小于等于缓冲区的深度值时通过测试
GL_GREATER|在片段深度值大于缓冲区的深度值时通过测试
GL_NOTEQUAL|在片段深度值不等于缓冲区的深度值时通过测试
GL_GEQUAL|在片段深度值大于等于缓冲区的深度值时通过测试

如果通过了测试, 会怎样? 两种情况

1. 将深度缓冲区的值更新为片段的深度值
2. 不做任何处理

### 测试通过后的处理函数

glDepthMask(GLboolean flag)

- flag 只可能是 GL_FALSE 或者 GL_TRUE
  
glDepthMask(GL_FALSE); 表示深度缓冲区是只读的, 不会被更改.

## 深度值精度 precision

线性计算深度值, 区间[0.0, 1.0]

$$
F_{depth} = \frac{z-near}{far-near}
$$

near/far分别是视锥中近平面/远平面的距离. 我们在投影矩阵 projection 设置时就指定的值. z 表示到摄像机的距离.

我们在顶点Shader中完成了坐标系转换 projection * view * model * vertex

在片段Shader中拿到的内置变量 gl_FragCoord.z 深度值却不是我们想象的这个线性值. 它的公式则是非线性的:

$$
gl_FragCoord.z = \frac{\frac{1}{z}-\frac{1}{near}}{\frac{1}{far}-\frac{1}{near}}
$$

这个公式保证了, 距离摄像机越远, 深度值会迅速向1移动(一个沿X镜像过来并上移1个单位的倒数函数f(x)=1/x). 以此加强纵深感. 

这样的深度值可以直接对应物体的精度:

靠近眼睛的物体, 颜色越深(RGB低).

## 屏幕空间坐标可视化

```GLSL
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform float SCR_WIDTH;
uniform float SCR_HEIGHT;

void main()
{
    FragColor = vec4(gl_FragCoord.x/SCR_WIDTH, gl_FragCoord.y/SCR_HEIGHT, gl_FragCoord.z, 1.0);
}
```

观察颜色从暗(近)到亮(远)的非线性变化.

并且可以通过公式逆推线性的可视化是怎样的:

令

$$
d = gl_FragCoord.z = \frac{1/z-1/near}{1/far-1/near}
$$

得到

$$
z = \frac{far*near}{far+d*near-d*far}
$$

带入线性公式得到

$$
d' = \frac{z-near}{far-near} = \frac{d*near}{far+d*near-d*far}
$$

可视化线性变换会有一个很直观的感受, 因为远平面设置非常远, 所以你要跑到非常远, 用屏幕右上角去观察片段, 而且要在片面几乎快要被裁剪掉的那一刻, 才会显示成白色(其他均不是白色). 通过颜色观察, 这种纵深感非常弱, 而且要渲染的细节非常多(视锥截取的屏幕投影的所有片段均享受同等精度显示的待遇), 这是不科学的(不论从人眼焦点的特点, 还是程序的开销上). 在人眼视野中, 只会看清焦点对准的物体(高精度), 虽然其他物体也在视野类(没有被裁减), 因为没有焦点对齐, 是一种模糊的状态(低精度), 这刚好解释了深度值的非线性方程意义.

## 深度冲突 Z-fighting

原因: 共面 Coplanar

深度冲突不能够被完全避免, 只能尽量规避:

1. 远不要把多个物体摆得太靠近，以至于它们的一些三角形会重叠。
2. 尽可能将近平面设置远一些。
3. 牺牲一些性能，使用更高精度的深度缓冲。大部分的显卡都支持32位的深度缓冲，这将会极大地提高精度。