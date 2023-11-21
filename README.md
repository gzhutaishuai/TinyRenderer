# TinyRenderer

基于https://github.com/ssloy/tinyrenderer的一个小型的c++软光栅渲染器

实现的基本功能

1.Bresham算法画直线(避免了浮点型的运算)。

![image text](https://github.com/gzhutaishuai/TinyRenderer/blob/main/%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE%202023-11-21%20161715.png)

2.古老版本单线程下如何画三角形。

3.基本的z-buffer算法。

4.相机坐标系的转换，透视投影，视口变换，透视除法的实现

5.flat Shading，Phong Shading，Blinn-Phong Shading

6.法线贴图
