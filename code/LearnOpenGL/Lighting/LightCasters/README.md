## 概念

投光物 Light Casters。

提供光照的物体。这个其实是Unity入门教程就要接触的东西了。

PS. 我们前面教程用的光源类型一直都是点光源。

## 平行光/直射光/定向光 Directional Light

属性：方向。

假设光源处于无限远处的模型，来自光源的每条光线就会近似于互相平行

它的所有光线都有着相同的方向，它的位置不重要。

例子————太阳。因为可以假设其光线是从无穷远射来的。

方向向量就会像这样来表示：vec4(0.2f, 1.0f, 0.3f, 0.0f)。这也可以作为一个快速检测光照类型的工具：你可以检测w分量是否等于1.0，来检测它是否是光的位置向量；w分量等于0.0，则它是光的方向向量。

```GLSL
if(lightVector.w == 0.0) // 注意浮点数据类型的误差
  // 执行定向光照计算
else if(lightVector.w == 1.0)
  // 根据光源的位置做光照计算（与上一节一样）
```

## 点光源 Point Light

属性：位置。

例子————灯泡、火把。在大部分的3D模拟中，我们都希望模拟的点光源仅照亮光源附近的区域而不是整个场景。

和之前教程用到的光源不同，点光源还需要考虑**强度的衰减(Attenuation)**。

在现实世界中，灯在近处通常会非常亮，但随着距离的增加光源的亮度一开始会下降非常快，但在远处时剩余的光强度就会下降的非常缓慢了。所以，我们需要一个不同的公式来减少光的强度。（像是一种指数梯度，非线性）。

$$
F_{att} = \frac{1.0}{K_c+K_l*d+K_q*d^2}
$$

在这里d代表了片段距光源的距离。接下来为了计算衰减值，我们定义3个（可配置的）项：常数项$K_c$、一次项$K_l$和二次项$K_q$。

- 常数项通常保持为1.0，它的主要作用是保证分母永远不会比1小，否则的话在某些距离上它反而会增加强度，这肯定不是我们想要的效果。
- 一次项会与距离值相乘，以线性的方式减少强度。
- 二次项会与距离的平方相乘，让光源以二次递减的方式减少强度。二次项在距离比较小的时候影响会比一次项小很多，但当距离值比较大的时候它就会比一次项更大了。

### $K_c$和$K_q$的设置

确地设定它们的值取决于很多因素：环境、希望光覆盖的距离、光的类型等。在大多数情况下，这都是经验的问题，以及适量的调整。

|距离|常数项|一次项|二次项|
|--|--|--|--|
7|1.0|0.7|1.8
13|1.0|0.35|0.44
20|1.0|0.22|0.20
32|1.0|0.14|0.07
50|1.0|0.09|0.032
65|1.0|0.07|0.017
100|1.0|0.045|0.0075
160|1.0|0.027|0.0028
200|1.0|0.022|0.0019
325|1.0|0.014|0.0007
600|1.0|0.007|0.0002
3250|1.0|0.0014|0.000007

## 聚光灯 Spotlight

属性：位置，方向，视野。

聚光灯和摄像机很像，有一个很明确的发光的位置，朝向哪个方向，而且有一个切光角(Cutoff Angle)和摄像机的FOV概念很像（FOV也是角度）。

光锥和视锥Frustum的区别也就是：

1. 形状上。前者是个圆锥体 circular cone，后者是个角锥体 pyramid，还是个四棱锥 quadrangular pyramid, 类似金字塔那样。
2. 计算上。前者需要考虑距离对强度的衰减。后者只关心观察矩阵和投影矩阵的变换，一部分衰减计算由【多级渐远纹理 mipmap】来表现，包括裁剪都交给OpenGL自动完成。

### 平滑/软化边缘

已知的Cutoff Angle定义为内圆锥角度$arccos(ϕ)$

定义外圆锥角度$arccos(γ)$, 若反射光角度在这两个角度之间, 就做出淡出效果. 这其实是个线性插值公式:

$$
I = \frac{θ−γ}{ϕ-γ} 
$$

$I$就是强度, 其他的符号表示的都是余弦值(满足角越大,余弦值越小的规律). $γ$的取值依然是经验值. 令$ε=ϕ-γ$


|θ|θ（角度）|ϕ（内光切）|ϕ（角度）|γ（外光切）|γ（角度）|ϵ|I|
|--|--|--|--|--|--|--|--|
0.87|30|0.91|25|0.82|35|0.91 - 0.82 = 0.09|0.87 - 0.82 / 0.09 = 0.56
0.9|26|0.91|25|0.82|35|0.91 - 0.82 = 0.09|0.9 - 0.82 / 0.09 = 0.89
0.97|14|0.91|25|0.82|35|0.91 - 0.82 = 0.09|0.97 - 0.82 / 0.09 = 1.67
0.83|34|0.91|25|0.82|35|0.91 - 0.82 = 0.09|0.83 - 0.82 / 0.09 = 0.11
0.64|50|0.91|25|0.82|35|0.91 - 0.82 = 0.09|0.64 - 0.82 / 0.09 = -2.0
0.966|15|0.9978|12.5|0.953|17.5|0.966 - 0.953 = 0.0448|0.966 - 0.953 / 0.0448 = 0.29