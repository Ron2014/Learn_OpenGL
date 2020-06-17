## 概念

1. 颜色缓冲 GL_COLOR_BUFFER_BIT
2. 深度缓冲 GL_DEPTH_BUFFER_BIT
3. 模板缓冲 GL_STENCIL_BUFFER_BIT

## 使用步骤

```
每个窗口库都需要为你配置一个模板缓冲。GLFW自动做了这件事，所以我们不需要告诉GLFW来创建一个，但其它的窗口库可能不会默认给你创建一个模板库，所以记得要查看库的文档。
```

### 启用模板测试 Stencil Testing

默认是禁用的

glEnable(GL_STENCIL_TEST);

### 刷新缓冲区

glClear(GL_STENCIL_BUFFER_BIT);

**刷新之后, 缓冲区初始值为0.**

### 设置比较函数

比较函数, 是将缓冲区的所有值与参考值ref进行比较.

glStencilFunc(GLenum func, GLint ref, GLuint mask)

- func 比较的操作码. 选项有：GL_NEVER、GL_LESS、GL_LEQUAL、GL_GREATER、GL_GEQUAL、GL_EQUAL、GL_NOTEQUAL和GL_ALWAYS。
- ref 比较的操作数. 参考值 Reference Value
- mask 掩码. 比较之前, 会和缓冲区数值/参考值进行与操作. 一般情况下都是 0xFF

- 左操作数: 模板缓冲区的值 & mask 
- 右操作数: 参考值 ref

### 设置处理函数

处理函数, 针对不同的比较结果处理缓冲区.

glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)

- sfail: 模板测试失败时对模板缓冲去采取的操作。
- dpfail: 模板测试通过，但深度测试失败时对模板缓冲去采取的操作。
- dppass: 模板测试和深度测试都通过时对模板缓冲去采取的操作。
  
操作包括

|行为|描述|
|--|--|
GL_KEEP|保持当前储存的模板值
GL_ZERO|将模板值设置为0
GL_REPLACE|将模板值设置为glStencilFunc函数设置的ref值
GL_INCR|如果模板值小于最大值则将模板值加1
GL_INCR_WRAP|与GL_INCR一样，但如果模板值超过了最大值则归零
GL_DECR|如果模板值大于最小值则将模板值减1
GL_DECR_WRAP|与GL_DECR一样，但如果模板值小于0则将其设置为最大值
GL_INVERT|按位翻转当前的模板缓冲值

### 设置缓冲区数值的掩码 BitMap

glStencilMask

```c
glStencilMask(0xFF); // 每一位写入模板缓冲时都保持原样
glStencilMask(0x00); // 每一位在写入模板缓冲时都会变成0
```

glStencilMask(0x00) 保证了缓冲区更新的部分, 只会更新为0.

## 遗留问题

利用Stencil Testing 模板测试, 如何能做到

- 不规则物体的轮廓(利用法线)
- 描边型轮廓的卡通渲染(Toon Shading)实现方案
- 物体被遮挡时的透视阴影. 比如游戏中开启红外线眼镜, 看见墙背后的敌人的(填充型)轮廓.