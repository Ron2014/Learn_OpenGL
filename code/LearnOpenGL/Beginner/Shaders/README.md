## GLSL语法结构

```GLSL
#version version_number
in type in_variable_name;       // in 表示输入
in type in_variable_name;

out type out_variable_name;     // out 表示输出

uniform type uniform_name;

int main()
{
  // 处理输入并进行一些图形操作
  ...
  // 输出处理过的结果到输出变量
  out_variable_name = weird_stuff_we_processed;
}
```

## 数据类型 type

基本C类型: int, float, double, unsigned int, bool.

unsigned int 可以简写成 uint.

此外就是添加了不同类型, 不同维度的向量数据类型 XvecN.

1. vecN     - float
2. bvecN    - boolean
3. ivecN    - int
4. uvecN    - unsigned int
5. dvecN    - double

N代表分量的数量, 也就是向量的`维度` dimensionality.

### 分量访问

分量访问形式 vec.X, 这个X可以时下面字符对应

- 向量访问 xyzw
- 颜色访问 rgba
- 纹理坐标访问 stpq
  
### 分量重组 swizzling

```
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyw;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
```

### 在构造函数中补全分量

从传入的向量中获取足够的分量, 剩下的分量通过传值补全.

```
vec2 vect = vec2(0.5, 0.7);
vec4 result = vec4(vect, 0.0, 0.0);
vec4 otherResult = vec4(result.xyz, 1.0);
```

## 输入输出

```
顶点着色器应该接收的是一种特殊形式的输入，否则就会效率低下。顶点着色器的输入特殊在，它从顶点数据中直接接收输入。为了定义顶点数据该如何管理，我们使用location这一元数据指定输入变量，这样我们才可以在CPU上配置顶点属性。我们已经在前面的教程看过这个了，layout (location = 0)。顶点着色器需要为它的输入提供一个额外的layout标识，这样我们才能把它链接到顶点数据。
```

话说, layout (location = 0) 不是定义顶点属性的序号么? 反正我时这么理解的.

此处定义影响到外部调用

```c
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
```

这两个接口的第一个参数, 要与之对应.

## uniform

uniform, 统一的, 一致的, 始终如一的. 它在不同的着色器程序/对象中都是唯一的, 共享的变量.