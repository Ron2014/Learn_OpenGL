## 概念

### UV映射 uv-mapping

- 模型(Model)导入(Import)
 
常见3D建模工具(3D Modeling Tool):

- [Blender](http://www.blender.org/)
- [3DS Max](http://www.autodesk.nl/products/3ds-max/overview)
- [Maya](http://www.autodesk.com/products/autodesk-maya/overview)

这些工具将会在导出到模型文件的时候自动生成所有的顶点坐标、顶点法线以及纹理坐标.所有的技术细节都隐藏在了导出的模型文件中。但是，作为图形开发者，我们就必须要了解这些技术细节了: 我们的工作就是**解析这些导出的模型文件以及提取所有有用的信息，将它们储存为OpenGL能够理解的格式**。

像是Wavefront的.obj这样的模型格式，只包含了模型数据以及材质信息，像是模型颜色和漫反射/镜面光贴图。而以XML为基础的Collada文件格式则非常的丰富，包含模型、光照、多种材质、动画数据、摄像机、完整的场景信息等等。

Wavefront的.obj格式通常被认为是一个易于解析的模型格式。建议至少去Wavefront的wiki页面上看看文件格式的信息是如何封装的。

不同种类的文件格式有很多，它们之间通常并没有一个通用的结构。所以如果我们想从这些文件格式中导入模型的话，我们必须要去自己对每一种需要导入的文件格式写一个导入器。

## Assimp (Open Asset Import Library) 开放的资产导入库

当使用Assimp导入一个模型的时候，它通常会将整个模型加载进一个场景(Scene)对象，它会包含导入的模型/场景中的所有数据。Assimp会将场景载入为一系列的节点(Node)，每个节点包含了场景对象中所储存数据的索引，每个节点都可以有任意数量的子节点。

### 网格(Mesh)

包含了渲染所需要的所有相关数据，像是 一组<顶点位置+法向量+纹理坐标>(可以称为`顶点属性或顶点数据`)、一组<面(Face)数据>和一个物体的<材质对象索引>.

### 面(Face)

一个网格包含了多个面。Face代表的是物体的渲染图元(Primitive)（三角形、方形、点）。一个面包含了组成图元的顶点的索引。这些索引就和顶点属性数组具有对应关系.

### 材质对象(Material)

Material对象包含了一些函数能让我们获取物体的材质属性，比如说颜色和纹理贴图（比如漫反射和镜面光贴图）. 一个网格会记录一个材质对象, 只是这个材质对象具体的数据存储在Scene中, 网格记录的是它的索引.

### 步骤

1. 使用[Assimp](https://github.com/assimp/assimp)将一个物体加载Scene对象中.
2. 递归遍历节点, 获取对应的Mesh对象.
3. 处理每个Mesh对象, 获取顶点属性/面(索引)/材质属性
4. 最终得到一些列Mesh数据, 将他们(组合并)存在一个Model对象中.

通常每个模型都由几个子模型/形状组合而成。组合模型的每个单独的形状就叫做一个网格(Mesh)。比如说有一个人形的角色：艺术家通常会将头部、四肢、衣服、武器建模为分开的组件，并将这些网格组合而成的结果表现为最终的模型。一个网格是我们在OpenGL中绘制物体所需的最小单位（顶点数据、索引和材质属性）。一个模型（通常）会包括多个网格。

如果我们想要绘制一个模型，我们不需要将整个模型渲染为一个整体，只需要渲染组成模型的每个独立的网格就可以了。
