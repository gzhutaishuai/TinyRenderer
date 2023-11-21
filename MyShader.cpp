#include <cmath>
#include <limits>
#include <cstdlib>
#include "MyShader.h"

Matrix modelMatrix;//相机坐标系
Matrix viewPort;//视口变换矩阵
Matrix Projection;//透视投影矩阵

IShader::~IShader(){}

// 视口变换
void ViewPort(int x, int y, int w, int h)
{
    //初始化单位矩阵
    viewPort = Matrix::identity();
    viewPort[0][3] = x + w / 2.f;
    viewPort[1][3] = y + h / 2.f;
    viewPort[2][3] = 255.0f / 2.f;

    viewPort[0][0] = w / 2.f;
    viewPort[1][1] = h / 2.f;
    viewPort[2][2] = 255.0f/ 2.f;
}
//透视投影
void projectionMatrix(float z)
{
    //这里的透视投影矩阵特殊，是因为相机是放在z轴上看的
    Projection = Matrix::identity();
    Projection[3][2] =z;//   -1/c
}

void projectionDivied()
{
    Projection[0][0] = Projection[0][0] / Projection[0][3];
    Projection[0][1] = Projection[0][1] / Projection[1][3];
    Projection[0][2] = Projection[0][2] / Projection[2][3];
    Projection[3][0] = 1.0f;
}

//坐标系变换
void LookAt(Vec3f eye_position, Vec3f center, Vec3f up)
{
    Vec3f v = (eye_position-center).normalize();//z
    Vec3f r = cross(up, v).normalize();//x
    Vec3f u = cross(v, r).normalize();//y

    modelMatrix = Matrix::identity();
    //T的逆乘以R的逆，坐标的转换
    for (int i = 0; i < 3; i++)
    {
        modelMatrix[0][i] = r[i];//rx,ry,rz
        modelMatrix[1][i] = u[i];//ux,uy,uz
        modelMatrix[2][i] = v[i];//vx,vy,vz
        modelMatrix[i][3] = -center[i];
    }
}

//static std::tuple<float, float, float> computeBarycentric2D(const Vec4f* pts, float x, float y)
//{
// /*   float Xa = pts[0][0]/pts[0][3];
//    float Xb = pts[1][0]/pts[1][3];
//    float Xc = pts[2][0]/pts[2][3];
//    float Ya = pts[0][1]/pts[0][3];
//    float Yb = pts[1][1]/pts[1][3];
//    float Yc = pts[2][1]/pts[2][3];*/
//
//    int Xa = pts[0][0] ;
//    int Xb = pts[1][0] ;
//    int Xc = pts[2][0] ;
//    int Ya = pts[0][1] ;
//    int Yb = pts[1][1] ;
//    int Yc = pts[2][1] ;
//
//    float alpha = static_cast<float>((x - Xc) * (Yc - Yb) - (y - Yc) * (Xc - Xb)) / static_cast<float>((Xa - Xc) * (Yc - Yb) - (Ya - Yc) * (Xc - Xb));
//    float beta = static_cast<float>((Xa - Xc) * (y - Yc) + (Ya - Yc) * (Xc - x)) / static_cast<float>((Xc - Xb) * (Ya - Yc) - (Yc - Yb) * (Xa - Xc));
//    float gamma = 1 - alpha - beta;
//
//    return { alpha, beta, gamma };
//}

/// <summary>
/// 求重心坐标
/// </summary>
/// <param name="A">三角形顶点1</param>
/// <param name="B">三角形顶点2</param>
/// <param name="C">三角形顶点3</param>
/// <param name="P">要判断的点</param>
/// <returns></returns>
static std::tuple<float, float, float> computeBarycentric2D(Vec2i A,Vec2i B,Vec2i C, Vec2i P)
{
    //
    Vec3f Triangle[2];
        for (int i = 2; i--; ) 
        {
            Triangle[i][0] = C[i] - A[i];
            Triangle[i][1] = B[i] - A[i];
            Triangle[i][2] = A[i] - P[i];
        }
        //float alpha = static_cast<float>((x - Xc) * (Yc - Yb) - (y - Yc) * (Xc - Xb)) / static_cast<float>((Xa - Xc) * (Yc - Yb) - (Ya - Yc) * (Xc - Xb));
        //float beta = static_cast<float>((Xa - Xc) * (y - Yc) + (Ya - Yc) * (Xc - x)) / static_cast<float>((Xc - Xb) * (Ya - Yc) - (Yc - Yb) * (Xa - Xc));
        //float gamma = 1 - alpha - beta;

        Vec3f u = cross(Triangle[0], Triangle[1]);//叉乘
        if (std::abs(u[2]) > 1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
            return { 1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z };//alpha,beta,gamma
            
        return { -1,1,1 }; // in this case generate negative coordinates, it will be thrown away by the rasterizator
    
}
void triangle(Vec4f* pts, IShader& shader, TGAImage& image, TGAImage& zbuffer)
{
    //求包围盒
    Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 2; j++) {
                bboxmin[j] = std::min(bboxmin[j], pts[i][j] / pts[i][3]);
                bboxmax[j] = std::max(bboxmax[j], pts[i][j] / pts[i][3]);
            }
        }
    for (int i =bboxmin.x; i <=bboxmax.x; i++)
    {
        for (int j = bboxmin.y; j <=bboxmax.y; j++)
        {
            //要判断的点P，注意，此时还不能确定点P在不在三角形内
            Vec2i P(i, j);
            TGAColor color;//颜色

            //计算重心坐标
            auto [alpha, beta, gamma] = computeBarycentric2D(proj<2>(pts[0] / pts[0][3]), proj<2>(pts[1] / pts[1][3]), proj<2>(pts[2] / pts[2][3]), proj<2>(P));
            //如果三者都比0.01小，说明点P不在三角形内
            if (alpha < -0.01 || beta < -0.01 || gamma < -0.01) continue;
            float z_interpolation = alpha * pts[0][2] + beta * pts[1][2] + gamma * pts[2][2];//对z查找
            float w_interpolation = alpha * pts[0][3] + beta * pts[1][3] + gamma * pts[2][3];//对w插值
            //这里先对z插值计算深度，又对w插值计算深度，然后用z/w+0.5来用zbuffer算法判断。加0.5避免隐式截断
            int frag_depth = std::max(0, std::min(255, int(z_interpolation / w_interpolation + 0.5)));//除以w的插值，得到正确的深度值

            //如果之前存储的z值大于现在的深度，不进行着色
            if (zbuffer.get(i, j)[0] > frag_depth)
            {
                continue;
            }
            Vec3f c(alpha, beta, gamma);//把插值保存下来
            //这里是回到main.cpp中进行一个像素颜色强度的插值
            bool discard = shader.fragment(c, color);
            if (!discard)
            {
                //更新当前像素点深度值
                zbuffer.set(P.x, P.y, TGAColor(frag_depth));
                //着色
                image.set(P.x, P.y, color);
            }
        }

    }
}

