## 概念

帧缓冲 Framebuffers = 颜色缓冲(Color Buffer) + 模板缓冲(Stencil Buffer) + 深度缓冲(Depth Buffer)

这些缓冲的数据占用内存.

帧缓冲可以自定义.

## 步骤

### 创建

创建一个帧缓冲对象 Framebuffer Object (FBO 存于OpenGL后台), 返回该对象的标识符.

```c
unsigned int fbo;
glGenFramebuffers(1, &fbo);
```

### 绑定对象

```c
glBindFramebuffer(GL_FRAMEBUFFER, fbo);
``

通过标识符将对象绑定到GL_FRAMEBUFFER目标, 将这个帧缓冲激活.

之后，所有的读取和写入帧缓冲的操作将会影响当前绑定的帧缓冲。

#### 分别绑定

- GL_READ_FRAMEBUFFER   帧缓冲提供给glReadPixels等读取操作使用
- GL_DRAW_FRAMEBUFFER   帧缓冲提供给渲染/清除等写入操作使用

大部分情况你都不需要区分它们，通常都会使用GL_FRAMEBUFFER，绑定到两个上。

### 填写完整的帧缓冲信息

一个完整的帧缓冲需要满足以下的条件：

1. 附加至少一个缓冲（颜色、深度或模板缓冲）。
2. 至少有一个颜色附件(Attachment)。
3. 所有的附件都必须是完整的（保留了内存）。
4. 每个缓冲都应该有相同的样本数(后面讲)。

检查(当前绑定的)帧缓冲信息是否完整:

```c
if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
  // 执行胜利的舞蹈
```

为帧缓冲创建一些附件，并将附件附加到帧缓冲上。之后所有的渲染操作将会渲染到当前绑定帧缓冲的附件中。

#### 离屏渲染(Off-screen Rendering)

由于我们的帧缓冲不是默认帧缓冲，渲染指令将不会对窗口的视觉输出有任何影响。

所有帧缓冲操作接触后, 通过绑定0激活默认缓冲, 让所有渲染操作在主窗口上有视觉效果. 再删除该对象

```c
glBindFramebuffer(GL_FRAMEBUFFER, 0);
glDeleteFramebuffers(1, &fbo);
```

#### 纹理附件

填写帧缓冲信息就是附加附件.

如果把一个纹理附加到帧缓冲的时候，所有的渲染指令将会写入到这个纹理中. 

优点是，所有渲染操作的结果将会被储存在一个纹理图像中，我们之后可以在着色器中很方便地使用它。

和创建普通纹理很像, 区别就是:

1. 少了文件加载的流程, 也就没有data.
2. 没有图片数据, 颜色模式也就随意写成 GL_RGB
3. 纹理宽高与屏幕尺寸对齐.
4. 不需要关心环绕方式或多级渐远纹理

```c
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glGenerateMipmap(GL_TEXTURE_2D);

  // 为当前绑定的纹理对象设置环绕、过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

附加函数 

```c
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
```

- target：    帧缓冲的目标（绘制、读取或者两者皆有）
- attachment：我们想要附加的附件类型。当前我们正在附加一个颜色附件。注意最后的0意味着我们可以附加多个颜色附件。我们将在之后的教程中提到。
- textarget： 你希望附加的纹理类型
- texture：   要附加的纹理本身
- level：     多级渐远纹理的级别。我们将它保留为0。

纹理附件区分为

1. 颜色缓冲纹理附件
2. 深度缓冲纹理附件
3. 模板缓冲纹理附件
4. 深度+模板缓冲纹理附件

深度缓冲

```c
...
glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
...
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
```

模板缓冲

```c
...
glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, SCR_WIDTH, SCR_HEIGHT, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, NULL);
...
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
```

深度缓冲(24位) + 模板缓冲(8位) : GL_DEPTH24_STENCIL8 ... GL_UNSIGNED_INT_24_8

```c
// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
```

#### 渲染缓冲(Renderbuffer)对象附件

渲染缓冲对象附加的好处是，它会将数据储存为OpenGL原生的渲染格式，它是为离屏渲染到帧缓冲优化过的。

渲染缓冲对象通常都是只写的. 它的数据已经是原生的格式，所以，交换缓冲这样的操作在使用渲染缓冲对象时会非常快。

使用条件: 只有颜色缓冲是采样(读取)的, 深度和模板缓冲不需要采样.

1. 创建渲染缓冲对象

```c
// 1. 生成纹理
// 2. 将它附加到当前绑定的帧缓冲对象

unsigned int rbo;
glGenRenderbuffers(1, &rbo);
glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);  
glBindRenderbuffer(GL_RENDERBUFFER, 0);
```

内部格式设置为GL_DEPTH24_STENCIL8

2. 附加渲染缓冲对象

```c
glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
```

3. 检查并解绑

```c
if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

## 绘制场景到一个纹理上

步骤:

1. 激活自定义的帧缓冲, 像往常一样渲染场景(填充自定义帧缓冲)
2. 绑定默认的帧缓冲
3. 绘制屏幕尺寸的四边形, 采样自定义帧缓冲中的颜色缓冲, 用其填充默认的帧缓冲

最终效果是, 我们从镜头看到的信息和平常绘制一样, 只是这些信息其实是绘制在四边形上的.

```
渲染你的场景到不同的帧缓冲能够让我们在场景中加入类似镜子的东西，或者做出很酷的后期处理效果。
```

只是类似镜子, 其实是个投屏器/监视器, 即把摄像机拍到的东西, 绘制到一个电视机模型的屏幕上.

在一些演唱会的场景种会看到这样的效果.

```
似乎找不到将纹理附件附加到默认缓冲区, 然后用纹理附件绘制监视器这种, 前后都有深度信息的画法.
```

## 后期处理 Post-processing

后期处理的特点是, 可以对**全屏**的片段进行采样.

如果只是普通的片段着色器, 其片段都是通过顶点规划的几何图元变换而来的. 不能做到全屏的范围.