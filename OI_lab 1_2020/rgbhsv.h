#pragma once

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace rgbhsv
{
	void rgb_to_hsv(int r, int g, int b, float *r_h, float *r_s, float *r_v);
	void hsv_to_rgb(float h, float s, float v, int *r_r, int *r_g, int *r_b);
}
