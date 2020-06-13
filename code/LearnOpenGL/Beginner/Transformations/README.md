# 专业术语中英对照

- 矩阵 Matrix
- 变换 Transform
- 向量 Vector
- 方向 Direction
- 向量的大小/长度/强度/模 Magnitude
- 维度 Dimension
- 位置 Position
- 标量 Scalar
- 向量取反 Negate
- 分量 Component-wise
- 勾股/毕达哥拉斯定理 Pythagoras Theorem
- 单位向量 Unit Vector
- 标准化 Normalizing
- 正交 Orthogonal 两个向量互为直角
- 单位矩阵 Identity Matrix

单位矩阵

$$
\begin{bmatrix}
  1 & 0 & 0 & 0 \\
  0 & 1 & 0 & 0 \\
  0 & 0 & 1 & 0 \\
  0 & 0 & 0 & 1
\end{bmatrix}
$$

可以看成四个列向量的组合, 而且这四个列向量都是单位向量.

前3个向量构成了坐标系的3个轴, 第4个向量是个长度为0的齐次向量.

## 向量点乘 Dot Product

点乘是对各分量的积求和, 其结果是个标量.

$$
\begin{pmatrix}
   A_x \\
   A_y \\
   A_z
\end{pmatrix}
⋅
\begin{pmatrix}
   B_x \\
   B_y \\
   B_z
\end{pmatrix}
= A_x*B_x + A_y*B_y + A_z*B_z
$$

点乘同时等于 

$$
\bar{v}⋅\bar{k}=||\bar{v}||⋅||\bar{k}||⋅cosθ
$$

单位向量的点乘, 直接等于 cosθ.

向量点乘可以解决的问题: 计算两个向量之间的`夹角`.

更进一步的, 计算向量在另一个向量(方向)的`投影`.

## 向量叉乘 Cross Product

叉乘结果是正交于两个输入向量的第三个向量.

$$
\begin{pmatrix}
   A_x \\
   A_y \\
   A_z
\end{pmatrix}
×
\begin{pmatrix}
   B_x \\
   B_y \\
   B_z
\end{pmatrix}
= 
\begin{pmatrix}
A_y*B_z - A_z*B_y \\
A_z*B_x - A_x*B_z \\
A_x*B_y - A_y*B_x \\
\end{pmatrix}
$$

正交方向与坐标系是左手系还是右手系保持一致.

其长度满足公式

$$
||\bar{v}×\bar{k}||=||\bar{v}||⋅||\bar{k}||⋅sinθ
$$

## 缩放 Scaling

不均匀缩放 Non-uniform Scale
均匀缩放 Uniform Scale

$$
\begin{bmatrix}
   S_x  & 0   & 0   & 0 \\
   0    & S_y & 0   & 0 \\
   0    & 0   & S_z & 0 \\
   0    & 0   & 0   & 1
\end{bmatrix}
⋅
\begin{pmatrix}
   x \\
   y \\
   z \\
   1
\end{pmatrix}
= 
\begin{pmatrix}
S_x * x \\
S_y * y \\
S_z * z \\
1
\end{pmatrix}
$$

第四个缩放向量仍然是1，因为在3D空间中缩放w分量是无意义的.

## 位移/平移 Translation

向量本身不能平移, 平移的是这个向量指向的终点(Points).

$$
\begin{bmatrix}
  1 & 0 & 0 & T_x \\
  0 & 1 & 0 & T_y \\
  0 & 0 & 1 & T_z \\
  0 & 0 & 0 & 1
\end{bmatrix}
⋅
\begin{pmatrix}
  x \\
  y \\
  z \\
  1
\end{pmatrix}
= 
\begin{pmatrix}
  x + T_x \\
  y + T_y \\
  z + T_z\\
  1
\end{pmatrix}
$$

### 齐次坐标 Homogeneous Coordinates

齐次坐标就是将一个原本是n维的向量用一个n+1维向量来表示，是指一个用于投影几何里的坐标系统，如同用于欧氏几何里的笛卡儿坐标一般。

齐次坐标要解决的问题: 
1. 明确区分向量和点，
2. 易用于进行仿射（线性）几何变换 Affine Transformation。

简单来说, 如果缺少w分量, 矩阵变换的公式是这样的(需要两个矩阵)

$$
\bar{P'} = \overgroup{M_1} ⋅ \bar{P} + \overgroup{M_2}
$$

其中, M_1完成缩放和旋转, M_2完成平移.

扩充分量之后, 这两个组合可以合并成一个矩阵表示, 减少了计算步骤.

$$
\bar{P'} = \overgroup{M_2} ⋅ \overgroup{M_1} ⋅ \bar{P}
$$

向量的w分量也叫齐次坐标。想要从齐次向量得到3D向量，我们可以把x、y和z坐标分别除以w坐标。

如果一个向量的齐次坐标是0，这个坐标就是方向向量(Direction Vector)，因为w坐标是0，这个向量就不能(通过矩阵相乘)位移。

## 旋转 Rotation

- 角度制 degree   0-360
- 弧度制 radians  0-2PI
- 旋转轴 Rotation Axis

沿轴向旋转: 使**该轴指向屏幕里**, 然后就有了一个平面2D坐标系, 该平面上有2个轴, 分别作单位向量, 这2个向量就是单位矩阵中对应的列向量, 对它们分别旋转角θ(注意象限的分布/角的旋转都是逆时针的), 然后通过对轴做投影, 从单位矩阵推导出旋转矩阵.

这里要强调一下, **以下的公式都是应用在左手坐标系中**.

可以看到, 沿哪个轴旋转, 旋转矩阵中对应的列向量就是单位向量(没有变换).
  
### 沿Z轴旋转 (钻机)

在左手坐标系中, Z轴本身就指向屏幕里, 所以这里的旋转矩阵和2D旋转几乎是一样的.
- Y轴↑
- X轴→

$$
\begin{bmatrix}
  cosθ  & -sinθ & 0 & 0 \\
  sinθ  & cosθ  & 0 & 0 \\
  0     & 0     & 1 & 0 \\
  0     & 0     & 0 & 1
\end{bmatrix}
·
\begin{pmatrix}
  x \\
  y \\
  z \\
  1
\end{pmatrix}
= 
\begin{pmatrix}
  cosθ*x - sinθ*y\\
  sinθ*x + cosθ*y\\
  z \\
  1
\end{pmatrix}
$$
  
### 沿X轴旋转 (弯腰)

让X轴指向屏幕里, 这时候我们得到一个水平镜像的2D坐标系.
- Y轴↑
- Z轴←

$$
\begin{bmatrix}
  1 & 0     & 0     & 0 \\
  0 & cosθ  & -sinθ & 0 \\
  0 & sinθ  & cosθ  & 0 \\
  0 & 0     & 0     & 1
\end{bmatrix}
·
\begin{pmatrix}
  x \\
  y \\
  z \\
  1
\end{pmatrix}
= 
\begin{pmatrix}
  x \\
  cosθ*y - sinθ*z\\
  sinθ*y + cosθ*z\\
  1
\end{pmatrix}
$$

### 沿Y轴旋转 (朝向改变)

将Y轴向里, 我们得到一个垂直镜像的2D坐标系.
- Z轴↓
- X轴→

$$
\begin{bmatrix}
  cosθ  & 0 & sinθ  & 0 \\
  0     & 1 & 0     & 0 \\
  -sinθ & 0 & cosθ  & 0 \\
  0     & 0 & 0     & 1
\end{bmatrix}
·
\begin{pmatrix}
  x \\
  y \\
  z \\
  1
\end{pmatrix}
= 
\begin{pmatrix}
  cosθ*x + sinθ*z\\
  y \\
  -sinθ*x + cosθ*z\\
  1
\end{pmatrix}
$$

如果采用右手坐标系, 沿某轴旋转, 则该轴指向屏幕外(**始终保持旋转参考轴是正向的**), 这样求得的旋转矩阵都是一样的. 这里不再赘述, 感兴趣的可以自行推导. 参考[可汗学院给出了右手坐标系旋转矩阵的求解过程](https://www.khanacademy.org/math/linear-algebra/matrix-transformations/lin-trans-examples/v/rotation-in-r3-around-the-x-axis?modal=1).

我们有了坐标系3个轴的旋转矩阵之后, 就能通过复合计算实现自由旋转.

### 任意轴旋转矩阵

比如对于轴

$$
\begin{pmatrix}
R_x \\
R_y \\
R_z \\
\end{pmatrix}
$$

进行旋转

$$
\begin{bmatrix}
cosθ+R_x^{2}(1-cosθ)    & R_xR_y(1-cosθ)-R_zsinθ  & R_xR_z(1-cosθ)+R_ysinθ  & 0 \\
R_yR_x(1-cosθ)+R_zsinθ  & cosθ+R_y^{2}(1-cosθ)    & R_yR_z(1-cosθ)-R_xsinθ  & 0 \\
R_zR_x(1-cosθ)-R_ysinθ  & R_zR_y(1-cosθ)+R_xsinθ  & cosθ+R_z^{2}(1-cosθ) & 0 \\
0                       & 0                       & 0                       & 1
\end{bmatrix}
$$

即使这样一个矩阵也不能完全解决万向节死锁问题（尽管会极大地避免）。避免万向节死锁的真正解决方案是使用四元数(Quaternion)，它不仅更安全，而且计算会更有效率。

### 扩展阅读

[万向节死锁/万向锁](https://pensieve.cybeureka.com/2020/06/gimbal-lock/)

[四元数]()

## 反射/镜像 Reflection

产生镜像的变换.

- 2D: 参照物 - 某直线 y=ax+b (向量)
- 3D: 参照物 - 某平面 z=ax+by+c

方法:

1. 平移
2. 旋转
3. 反射
4. 逆旋转
5. 逆平移

## 错切/切变 Shear

改变对象形状. 

例如在Z方向错切

$$
\begin{bmatrix}
1   & 0   & sh_{zx} & 0 \\
0   & 1   & sh_{zy} & 0 \\
0   & 0   & 1       & 0 \\
0   & 0   & 0       & 1
\end{bmatrix} \\
x'=x+sh_{zx}z \\
y'=y+sh_{zy}z \\
$$

使得Z轴分量去影响其他轴的分量偏移.

错切具有一定的方向, 我们可以理解成它沿着某个参考线.

因此会存在一个平移分量.

$$
\begin{bmatrix}
1   & 0   & sh_{zx} & -sh_{zx}·z_{ref} \\
0   & 1   & sh_{zy} & -sh_{zy}·z_{ref} \\
0   & 0   & 1       & 0 \\
0   & 0   & 0       & 1
\end{bmatrix} \\
x'=x+sh_{zx}(z-z_{ref}) \\
y'=y+sh_{zy}(z-z_{ref}) \\
$$

PS. 这个真的不常用.

## 仿射变换 Affine transformation

满足公式(线性函数)

$$
x' = A_xx + B_xy + C_xz + D_x \\
y' = A_yx + B_yy + C_yz + D_y \\
z' = A_zx + B_zy + C_zz + D_z
$$

也就是一个自由的矩阵变换的过程

$$
\begin{bmatrix}
  A_x & B_x & C_x & D_x \\
  A_y & B_y & C_y & D_y \\
  A_z & B_z & C_z & D_z \\
  0   & 0   & 0   & 1
\end{bmatrix}
·
\begin{pmatrix}
x \\
y \\
z \\
1
\end{pmatrix}
= 
\begin{pmatrix}
x' \\
y' \\
z' \\
1
\end{pmatrix}
$$

可知, 平移 + 旋转 + 缩放 + 反射 + 错切 都是仿射变换的特例.

仿射变换 = 平移 + 旋转 + 缩放 + 反射 + 错切 的组合

## 矩阵组合

1. 列向量右乘矩阵. 从右向左, 才是矩阵变换的正确顺序
2. 建议组合矩阵时，先进行缩放操作，然后是旋转，最后才是位移，否则它们会（消极地）互相影响。