#include<cmath>
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Dense>
#include<iostream>
using namespace Eigen;

int main()
{
    Matrix2f m, n;
    m << 1, 2, 
        2, 3;
    n << 1, 1, 
        1, 1;
    
    // matrix multiply
    std::cout << "a * b =" << std::endl;
    std::cout << (m * n) << std::endl;

    // scale
    Vector2f v(1, 2);
    Matrix2f scale_mat;
    scale_mat(0, 0) = 2;
    scale_mat(1, 1) = 3;
    std::cout << "scale_mat * v = " << std::endl 
        << scale_mat * v << std::endl;

    int a = 2;
    // x' = x + y / a;
    // y' = y;
    Matrix2f shear_mat;
    shear_mat << 1, a,
                0, 1;
    std::cout << "shear_mat * v = " << std::endl 
        << shear_mat * v << std::endl;
}