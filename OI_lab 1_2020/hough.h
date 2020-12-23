#pragma once
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>

cv::Mat hough_ellipse_detection(cv::Mat img, std::vector<cv::Vec3b> &circles, float threshold,
    int neighbour_size, int rad_step, int center_step);

cv::Mat hough_circle_detection(cv::Mat img, std::vector<cv::Vec3b> &circles, float threshold,
    int neighbour_size, int rad_step, int center_step);

int approximate(float x, int step);

void draw_circle(cv::Mat & img, int a, int b, int r);

void print_detection_stat(std::vector<cv::Vec3b> &circles);

