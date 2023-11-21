#include <vector>
#include <iostream>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "MyShader.h"


Model* model = NULL;//加载模型
const int width = 800;//屏宽
const int height = 800;//屏高
const int depth = 255;//远近平面距离，这里默认255

Vec3f light_dir(1, 1, 1);//光源位置
Vec3f       eye(1, 1, 3);//相机位置
Vec3f    center(0, 0, 0);//要投影的位置
Vec3f        up(0, 1, 0);//相机位置的向上向量

//Phong
struct Shader : public IShader {
    mat<2, 3, float> varying_uv;//纹理
    mat<4, 4, float> uniform_M;//透视投影矩阵×modelview
    mat<4, 4, float> uniform_MIT;//逆转置
    virtual Vec4f vertex(int iface, int nthvert) 
    {
        varying_uv.set_col(nthvert, model->uv(iface, nthvert));//读入obj文件的纹理索引
        //读入顶点坐标
        Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert)); // read the vertex from .obj file
        //乘以view,projection,视口矩阵
        return  viewPort * Projection * modelMatrix * gl_Vertex; // transform it to screen coordinates
    }

    virtual bool fragment(Vec3f bar, TGAColor& color) 
    { 
        //对uv坐标进行插值
        Vec2f uv = varying_uv * bar;
         
        Vec3f n = proj<3>(uniform_MIT * embed<4>(model->normal(uv))).normalize();//n是shading point处的法向量
        Vec3f l = proj<3>(uniform_M * embed<4>(light_dir)).normalize();//l是入射光线向量
       /* Vec3f n = proj<3>( embed<4>(model->normal(uv))).normalize();
        Vec3f l = proj<3>(embed<4>(light_dir)).normalize();*/
        //Vec3f r = (n * (n * l * 2.0f) - l).normalize();//镜面反射角R   
        Vec3f h = (n+l).normalize();//半程向量
        //float spec = pow(std::max(r.z, 0.0f), model->specular(uv));//Phong高光
        float spec = pow(std::max((h*n), 0.0f), model->specular(uv));//Blinn_Phong
        float diff = std::max(0.0f, n * l);//漫反射,0说明光是从背面打过来的

        TGAColor c = model->diffuse(uv);
        color = c;
        for (int i = 0; i < 3; i++)
        {
            color[i] = std::min <float>(5 + c[i] * (diff + 0.6 * spec), 255);
        }
        return false;
        //这里不需要着色，着色任务放到triangle函数中
        //片段着色器可以丢弃当前像素的绘制，然后光栅器只需跳过它
                                    // no, we do not discard this pixel
    }
};


int main(int argc, char** argv) {
    if (2 == argc) {
        model = new Model(argv[1]);
    }
    else {
        model = new Model("D:/TinyR/Github tinyRnderer/Github tinyRnderer/african_head.obj");
    }

    //view Transform 变换到相机空间
    LookAt(eye, center, up);
    //投影变换，变换到裁剪空间
    projectionMatrix(-1.f / (eye - center).norm()); 
    //projectionDivied();
    //视口变换，变换到屏幕空间
    ViewPort(width / 8, height / 8, width * 3 / 4, height * 3 / 4);

    //光线
    light_dir.normalize();

    //初始化图像和深度值
    TGAImage image(width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

    //创建Shader类
    Shader shader;

    //因为如果对顶点进行仿射变换，对应的法向量并不是只要进行相应的放射变换就可以对应
    //而是需要推导公式，也就是仿射变换的矩阵M的转置+逆
    shader.uniform_M = Projection * modelMatrix;
    shader.uniform_MIT = (Projection * modelMatrix).invert_transpose();
    for (int i = 0; i < model->nfaces(); i++) {
        Vec4f screen_coords[3];//屏幕空间坐标
        for (int j = 0; j < 3; j++) {
            screen_coords[j] = shader.vertex(i, j);//读入屏幕坐标
        }
        //光栅化
        triangle(screen_coords, shader, image, zbuffer);
    }

    image.flip_vertically(); // to place the origin in the bottom left corner of the image
    zbuffer.flip_vertically();
    image.write_tga_file("output.tga");
    zbuffer.write_tga_file("zbuffer.tga");

    delete model;
    return 0;
}
