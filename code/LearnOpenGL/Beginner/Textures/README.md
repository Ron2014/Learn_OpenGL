## 纹理要解决的问题

不用生成过多的顶点, 但能通过图片+线性插值, 补齐足够的颜色.

## 纹理映射 Texture Map

顶点的坐标, 映射纹理的坐标.

如果纹理是一张布, 顶点是钉子, 纹理映射就像是我们准备在布的哪个位置下钉子, 钉子最终又要钉在墙上哪个位置.

而且钉好之后, 这张布可以自动缩放, 没有褶皱地显示.

其中:

顶点坐标                        - 钉子在墙上的位置

纹理坐标 Texture Coordinate     - 钉子钉在布上的位置

采样 Sampling / sample          - 采集片段颜色.

片段插值 Fragment Interpolation - 除顶点外其他位置的像素的颜色, 通过这个方法从纹理上获取对应的信息.


## 纹理坐标

范围[0, 1], 即原点在左下角.

顶点坐标则不同. 原点在屏幕中心, 范围[-1, 1].

## 纹理环绕方式 wrapping

| 环绕方式 | 描述 |
|---|---|
GL_REPEAT	          | 对纹理的默认行为。重复纹理图像。
GL_MIRRORED_REPEAT  | 和GL_REPEAT一样，但每次重复图片是镜像放置的。
GL_CLAMP_TO_EDGE    | 纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。
GL_CLAMP_TO_BORDER  | 超出的坐标为用户指定的边缘颜色。

glTexParameteri

1. 纹理类型, 通常 GL_TEXTURE_2D
2. 指定设置的选项与应用的纹理轴 ?
3. 环绕类型

如果是 GL_CLAMP_TO_BORDER, 还得设置边缘颜色

```c
float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
```

## 纹理过滤 Texture Filtering

纹理像素(Texture Pixel，也叫Texel).

很大的物体但是纹理的分辨率很低: 即纹理贴图很小, 但是要附着的表面很大(很多顶点映射到同一个纹理坐标).

纹理过滤要解决的问题: 通过纹理坐标确定采样的颜色.

- GL_NEAREST, 临近过滤 Nearest Neighbor Filtering. 取的是离纹理坐标最近的纹理像素, 从该像素取色.
- GL_LINEAR, 线性过滤 (Bi)linear Filtering. 纹理坐标附近的纹理像素, 计算插值来确定最终颜色(混合色).

GL_NEAREST颗粒感强, 不平滑(走样严重), 但这个风格就是所谓的像素风(8-bit).
GL_LINEAR颜色过度平滑, 但是看起来很糊(没办法, 前提已经说了是低分辨率的纹理).

放大(Magnify)和缩小(Minify)操作的时候可以设置纹理过滤.

## 多级渐远纹理 mipmap

多级渐远纹理要解决的问题: 物体太远时不需要过高的精度(分辨率), 此时可以替换成一个较低分辨率的纹理.

简单来说就是一系列的纹理图像，后一个纹理图像的边长是前一个的二分之一(即面积是1/4).

多级渐远纹理主要是使用在纹理被缩小的情况下.

对于纹理放大的情况, 多级渐远纹理和纹理过滤构成了4种组合.

|过滤方式 | 描述 |
|--|--|
GL_NEAREST_MIPMAP_NEAREST |使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
GL_LINEAR_MIPMAP_NEAREST  |使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
GL_NEAREST_MIPMAP_LINEAR  |在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
GL_LINEAR_MIPMAP_LINEAR   |在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样

OpenGL有一个glGenerateMipmaps函数，在创建完一个纹理后调用它OpenGL就会承担接下来的所有工作了。

## 加载与创建纹理

方式

1. 对于不同的图片格式, 编写图像加载器 image loader 将文件转换成字节序列.
2. 图像加载库 如 stb_image.h

## stb_image.h

单头 single header 文件图像加载库, like sol2.hpp.

通过定义STB_IMAGE_IMPLEMENTATION，预处理器会修改头文件，让其只包含相关的函数定义源码，等于是将这个头文件变为一个 .cpp 文件了。

加载图片函数

```c++
int width, height, nrChannels;
unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
```

## 采样器 Sampler

GLSL有一个供纹理对象使用的内建数据类型，叫做采样器(Sampler)，它以纹理类型作为后缀，比如sampler1D、sampler3D，或在我们的例子中的sampler2D.我们可以简单声明一个uniform sampler2D把一个纹理添加到片段着色器中，稍后我们会把纹理赋值给这个uniform。

我们使用GLSL内建的texture函数来采样纹理的颜色，它第一个参数是纹理采样器，第二个参数是对应的纹理坐标。texture函数会使用之前设置的纹理参数对相应的颜色值进行采样。这个片段着色器的输出就是纹理的（插值）纹理坐标上的(过滤后的)颜色。

## 纹理单元 Texture Unit

一个纹理的默认纹理单元是0，它是默认的激活纹理单元，所以教程前面部分我们没有分配一个位置值。所以 uniform 采样器有值.

纹理单元要解决的问题: 使得片段着色器支持多个纹理.

渲染循环中, 激活纹理单元 glActiveTexture:

```c++
glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
glBindTexture(GL_TEXTURE_2D, texture);
```

渲染循环前, 将采样器与纹理单元编号简历映射 glUniform1i:

```c++
ourShader.use(); // 别忘记在激活着色器前先设置uniform！
glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 手动设置
ourShader.setInt("texture2", 1); // 或者使用着色器类设置
```

## 图片Y轴方向

通常是↓, 翻转函数(垂直镜像):

```c++
stbi_set_flip_vertically_on_load(true);
```