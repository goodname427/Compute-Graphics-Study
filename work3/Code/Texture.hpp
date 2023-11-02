//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <eigen3/Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;

    Eigen::Vector3f getColor(int u, int v)
    {
        u = std::clamp(u, 0, width - 1);
        v = std::clamp(v, 0, height - 1);

        auto color = image_data.at<cv::Vec3b>(v, u);
       
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

    Eigen::Vector3f lerp(Eigen::Vector3f a, Eigen::Vector3f b, float t)
    {
        return a + ((b - a) * t);
    }
public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        int u_img = static_cast<int>(u * width);
        int v_img = static_cast<int>((1 - v) * height);
       
        return getColor(u_img, v_img);
    }

    Eigen::Vector3f getColorBilinear(float u, float v)
    {
        u = u * width;
        v = (1 - v) * height;
 
        float t1 = u - std::floor(u);
        float t2 = v - std::floor(v);

        //uv3 uv4
        //uv1 uv2
        auto color1 = getColor(static_cast<int>(std::floor(u)), static_cast<int>(std::floor(v)));
        auto color2 = getColor(static_cast<int>(std::ceil(u)), static_cast<int>(std::floor(v)));
        auto color3 = getColor(static_cast<int>(std::floor(u)), static_cast<int>(std::ceil(v)));
        auto color4 = getColor(static_cast<int>(std::ceil(u)), static_cast<int>(std::ceil(v)));

        auto color12 = lerp(color1, color2, t1);
        auto color34 = lerp(color3, color4, t1);

        auto color = lerp(color12, color34, t2);

        return color;
    }

};
#endif //RASTERIZER_TEXTURE_H
