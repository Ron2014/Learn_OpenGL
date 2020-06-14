## 专业术语对照表

- 散射(Scatter)。有些物体反射光的时候不会有太多的散射(Scatter)，因而产生一个较小的高光点，而有些物体则会散射很多，产生一个有着更大半径的高光点。
- 材质(Material)

## 概念

当描述一个物体的时候，我们可以用这三个分量来定义一个材质颜色(Material Color)：环境光照(Ambient Lighting)、漫反射光照(Diffuse Lighting)和镜面光照(Specular Lighting)。通过为每个分量指定一个颜色，我们就能够对物体的颜色输出有着精细的控制了。再添加反光度(Shininess)这个分量到上述的三个颜色中。

- 环境光照(Ambient Lighting)颜色    - objColor
- 漫反射光照(Diffuse Lighting)颜色  - objColor
- 镜面光照(Specular Lighting)颜色   - objColor + 反光度（影响镜面高光的散射/半径）
  
总的来说，我们是用 material 描述 objColor.