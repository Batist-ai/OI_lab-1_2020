#pragma once
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>


void canny(cv::Mat &src_image, cv::Mat &contours, int low, int high);
cv::Mat convert_to_grayscale(cv::Mat image);
cv::Mat sobel(cv::Mat image, cv::Mat &angles); // Собель
cv::Mat non_max_suppression(cv::Mat image, cv::Mat angles); // Подавление не максимальных значений
cv::Mat double_threshold(cv::Mat image, int low, int high); 