#pragma once

#include <opencv2/opencv.hpp>

typedef cv::Point3_<uint8_t> Pixel;

struct Pixel_Data
{
    int x;
    int y;
    Pixel pixel;
};