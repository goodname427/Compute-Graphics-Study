#include<cmath>
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Dense>
#include<iostream>
using namespace Eigen;

int main()
{
    // 给定一个点 P =(2,1), 将该点绕原点先逆时针旋转 45 ◦ ,再平移 (1,2), 计算出变换后点的坐标(要求用齐次坐标进行计算)。
    
    // init
    Vector2f p(2, 1);
    std::cout << "original point p:" << std::endl;
    std::cout << p << std::endl;
    
    // rotate
    Matrix2f rotate_mat;
    rotate_mat << cos(M_PI_2), -sin(M_PI_2), sin(M_PI_2), cos(M_PI_2);
    p = rotate_mat * p;
    std::cout << "rotated point p:" << std::endl;
    std::cout << p << std::endl;

    // translate
    p += Vector2f(1, 2);
    std::cout << "translated point p:" << std::endl;
    std::cout << p << std::endl;
}