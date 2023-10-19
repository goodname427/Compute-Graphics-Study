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
    
    std::cout << "a * b =" << std::endl;
    std::cout << (m * n) << std::endl;
    
}