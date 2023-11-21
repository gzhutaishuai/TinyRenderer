# TinyRenderer

基于https://github.com/ssloy/tinyrenderer
的一个小型的c++软光栅渲染器

实现的基本功能

1.Bresham算法画直线(避免了浮点型的运算)。

![image text](https://github.com/gzhutaishuai/TinyRenderer/blob/main/%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE%202023-11-21%20161715.png)

2.古老版本单线程下如何画三角形。

![image text](https://github.com/gzhutaishuai/TinyRenderer/blob/main/%E5%8D%95%E7%BA%BF%E7%A8%8B%E7%94%BB%E7%BA%BF.jpg)

3.基本的z-buffer算法。

![image text](https://github.com/gzhutaishuai/TinyRenderer/blob/main/z-buffer.jpg)

4.相机坐标系的转换，透视投影，视口变换，透视除法的实现

![image text](https://github.com/gzhutaishuai/TinyRenderer/blob/main/%E7%9B%B8%E6%9C%BA%2B%E5%85%89%E7%85%A7.jpg)

5.flat Shading，Phong Shading，Blinn-Phong Shading

![image text](https://github.com/gzhutaishuai/TinyRenderer/blob/main/Phong%E6%97%A0%E6%B3%95%E7%BA%BF%E8%B4%B4%E5%9B%BE.jpg)

6.法线贴图
Blinn_Phong模型

![image text](https://github.com/gzhutaishuai/TinyRenderer/blob/main/%E6%B3%95%E7%BA%BF%E8%B4%B4%E5%9B%BEBlinn-Phong.jpg)

Phong模型

![image text](https://github.com/gzhutaishuai/TinyRenderer/blob/main/%E6%B3%95%E7%BA%BF%E8%B4%B4%E5%9B%BEPhong.jpg)
