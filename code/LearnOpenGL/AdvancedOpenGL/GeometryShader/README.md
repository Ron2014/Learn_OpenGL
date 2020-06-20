## 概念

布局修饰符(Layout Qualifier)

```GLSL
layout (points) in;
```

### 输入类型

- points              点      (1)
- lines               线      (2)
- lines_adjacency             (4)
- triangles           三角面  (3)
- triangles_adjacency         (6)

### 输出类型

- points
- line_strip
- triangle_strip

### 最大能够输出的顶点数量

```
layout (line_strip, max_vertices = 2) out;
```

为了生成更有意义的结果，我们需要某种方式来获取前一着色器阶段的输出。GLSL提供给我们一个内建(Built-in)变量，它被声明为一个接口块（Interface Block）在内部看起来（可能）是这样的：

```
in gl_Vertex
{
    vec4  gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];
```

几何着色器的输入是一个图元的所有顶点。所以 gl_in 为数组.

每次我们调用EmitVertex时，gl_Position中的向量会被添加到图元中来。当EndPrimitive被调用时，所有发射出的(Emitted)顶点都会合成为指定的输出渲染图元。重复调用EndPrimitive能够生成多个图元。

**使用几何着色器, 可以通过有限的顶点(坐标)(, 以它为参考点生成更多的顶点)来自由绘制图形.**

体素(Voxel)

## 爆破 Explode

向内爆炸(Implode)

**爆破效果以图元为单位的操作, 给了粒子系统设计以启发.**

## 法向量可视化

检测法向量是否正确的一个很好的方式就是对它们进行可视化，几何着色器正是实现这一目的非常有用的工具。