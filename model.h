#pragma once
#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "geometry.h"
#include "tgaimage.h"

class Model {
private:
    std::vector<Vec3f> verts_;//三个顶点的索引
    //每个小三角形面片存储的数据类型为Vec3i,每个三角形由三个Vec3i组成，分别代表三个顶点
    //每个顶点存储三个整数值，分别对应顶点索引/纹理坐标索引/法线向量索引
    std::vector<std::vector<Vec3i> > faces_; 
    std::vector<Vec3f> norms_;//法线向量
    std::vector<Vec2f> uv_;//纹理坐标
    TGAImage diffusemap_;//漫反射贴图
    TGAImage normalmap_;//法线贴图
    TGAImage specularmap_;//高光贴图
    void load_texture(std::string filename, const char* suffix, TGAImage& img);
public:
    Model(const char* filename);
    ~Model();
    int nverts();//返回模型的顶点数量
    int nfaces();//返回模型的面片数量
    Vec3f normal(int iface, int nthvert);//返回法线向量
    Vec3f normal(Vec2f uv);
    Vec3f vert(int i);//返回第i个顶点
    Vec3f vert(int iface, int nthvert);//返回顶点坐标
    Vec2f uv(int iface, int nthvert);//返回纹理坐标
    TGAColor diffuse(Vec2f uv);//漫反射
    float specular(Vec2f uv);//高光处理
    std::vector<int> face(int idx);//返回第idx个面片
};
#endif //__MODEL_H__

