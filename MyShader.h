#pragma once
#include "tgaimage.h"
#include "geometry.h"

extern Matrix modelMatrix;
extern Matrix viewPort;
extern Matrix Projection;

//视口变换
void ViewPort(int x, int y, int w, int h);

//透视投影
void  projectionMatrix(float z = 0.0f);

//相机
void LookAt(Vec3f eye_positon, Vec3f center, Vec3f up);

//void projectionDivied();

struct IShader
{
	virtual ~IShader();
	virtual Vec4f vertex(int iface, int nthvert) = 0;
	virtual bool fragment(Vec3f bar, TGAColor& color) = 0;
};

//光栅化
void triangle(Vec4f* pts, IShader& shader, TGAImage& image, TGAImage& zbuffer);
