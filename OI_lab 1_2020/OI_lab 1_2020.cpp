//// OI_lab 1_2020.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
////

#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>
#include <chrono> 

#include "pixel.h"
#include "qmetrics.h"
#include "rgbhsv.h"
#include "filters.h"
#include "canny.h"
#include "hough.h"
#include "OI_lab 1_2020.h"


#define CLAMP(_v, _min, _max)\
	(_v < _min ? _min: _v > _max ? _max: _v)

using namespace cv;
using namespace std;
using namespace std::chrono;
using namespace qm;
using namespace rgbhsv;


/**
 * @function main
 * @brief Main function
 */
int main(int argc, char** argv)
{
	//part_one();
	//part_two();
	//part_three();
    // part_four();
    part_five();

	return 0;
}

void part_one()
{
	char img_1_name[] = "D:\\3e635.jpg";
	char img_1_gs_name[] = "D:\\3e635-gs.jpg";
	char img_1_cvgs_name[] = "D:\\3e635-cvgs.jpg";
	char img_2_name[] = "D:\\3e635a.jpg";
	Mat img_1 = imread(img_1_name, CV_LOAD_IMAGE_GRAYSCALE);
	//imshow("Test", img_1);
	//waitKey(6000);
	if (!img_1.data) {
		printf("Image not loaded.");
	}

	Mat img_2 = imread(img_2_name, CV_LOAD_IMAGE_GRAYSCALE);
	//imshow("Test", img_2);
	//waitKey(6000);	

	Scalar img_1_mean, img_1_dev, img_2_mean, img_2_dev;

	meanStdDev(img_1, img_1_mean, img_1_dev);
	meanStdDev(img_2, img_2_mean, img_2_dev);

	printf("avg1=%f, dev1=%f, avg2=%f, dev2=%f\n\n", img_1_mean.val[0], img_1_dev.val[0], img_2_mean.val[0], img_2_dev.val[0]);

	waitKey(0);

	compute_quality_metrics(img_1_name, img_2_name, 10);
}

void part_two()
{
	char img_1_name[] = "D:\\3e635.jpg";
	char img_1_gs_name[] = "D:\\3e635-gs.jpg";
	char img_1_cvgs_name[] = "D:\\3e635-cvgs.jpg";

	Mat img_1 = imread(img_1_name);
	//imshow("Test", img_1);
	//waitKey(6000);
	if (!img_1.data) {
		printf("Image not loaded.");
		exit(255);
	}
	imshow("src", img_1);
	waitKey(2000);

	// Загружаем с использованием преобразования в оттенки серого
	Mat img_1_cvgs = imread(img_1_name, CV_LOAD_IMAGE_GRAYSCALE);
	imshow("cvgs", img_1_cvgs);
	imwrite(img_1_cvgs_name, img_1_cvgs);
	waitKey(2000);

	// Преобразуем исходное изображение в оттенки серого используя average  (R + G + B) / 3
	Mat img_1_gs = img_1;
	typedef cv::Point3_<uint8_t> Pixel;
	img_1_gs.forEach<Pixel>([](Pixel &p, const int * position) -> void {
		uint value = (p.x + p.y + p.z) / 3;
		p.x = value;
		p.y = value;
		p.z = value;
	});

	imshow("gs", img_1_gs);
	imwrite(img_1_gs_name, img_1_gs);
	waitKey(2000);

	printf("\n\nCheck the difference between cvgs and gs images:\n");
	compute_quality_metrics(img_1_gs_name, img_1_cvgs_name, 10);

	// Преобразуем исходное изображение в оттенки серого используя функцию cvtColor
	Mat img_1_gs_cvtcolor;
	cvtColor(img_1, img_1_gs_cvtcolor, CV_BGR2GRAY);
	imshow("gs_cvtcolor", img_1_gs_cvtcolor);
	waitKey(2000);
	imwrite("D:\\3e635-gs-cvtcolor.jpg", img_1_gs_cvtcolor);
}
char RGB_window[30] = "RGB Window";
char HSV_window[30] = "HSV Window";
Mat src, hsv;

static void onMouse(int event, int x, int y, int f, void*) {
	Mat image = src.clone();
	Vec3b rgb = image.at<Vec3b>(y, x);
	int B = rgb.val[0];
	int G = rgb.val[1];
	int R = rgb.val[2];

	Mat HSV;
	Mat RGB = image(Rect(x, y, 1, 1)); //capture that pixel in its own ROI
	cvtColor(RGB, HSV, CV_BGR2HSV);

	Vec3b hsv = HSV.at<Vec3b>(0, 0);
	int H = hsv.val[0];
	int S = hsv.val[1];
	int V = hsv.val[2];

	char name[30];
	sprintf_s(name, "B=%d", B);
	putText(image, name, Point(150, 40), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 255, 0), 2, 8, false);

	sprintf_s(name, "G=%d", G);
	putText(image, name, Point(150, 80), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 255, 0), 2, 8, false);

	sprintf_s(name, "R=%d", R);
	putText(image, name, Point(150, 120), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 255, 0), 2, 8, false);

	sprintf_s(name, "H=%d", H);
	putText(image, name, Point(25, 40), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 255, 0), 2, 8, false);

	sprintf_s(name, "S=%d", S);
	putText(image, name, Point(25, 80), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 255, 0), 2, 8, false);

	sprintf_s(name, "V=%d", V);
	putText(image, name, Point(25, 120), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 255, 0), 2, 8, false);

	sprintf_s(name, "X=%d", x);
	putText(image, name, Point(25, 300), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 0, 255), 2, 8, false);

	sprintf_s(name, "Y=%d", y);
	putText(image, name, Point(25, 340), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 0, 255), 2, 8, false);
	//namedWindow("Ref HSV",WINDOW_NORMAL);
	Mat ref(50, 50, CV_8UC3, Scalar(H, S, V));
	//imwrite("hsv.jpg",image);
	imshow(RGB_window, image);
	//imshow( "Ref HSV",ref);

}

uchar clamp_uc(uint min, uint max, uint value)
{
	return (uchar)(value < min ? min : value > max ? max : value);
}

void bright(Mat src, Mat &result, int brightness)
{
	typedef cv::Point3_<uint8_t> Pixel;
	result = src.clone();
	result.forEach<Pixel>([brightness](Pixel &p, const int * position) -> void {
		p.x = clamp_uc(0, 255, (p.x + brightness));
		p.y = clamp_uc(0, 255, (p.y + brightness));
		p.z = clamp_uc(0, 255, (p.z + brightness));
	});
}

void bright_hsv(Mat src, Mat &result, int brightness)
{
	typedef cv::Point3_<uint8_t> Pixel;
	result = src.clone();
	result.forEach<Pixel>([brightness](Pixel &p, const int * position) -> void {
		p.z = clamp_uc(0, 255, (p.z + brightness));
	});
}
void part_three() 
{
	char img_1_name[] = "D:\\3e635.jpg";
	char img_1_gs_name[] = "D:\\3e635-gs.jpg";
	char img_1_cvgs_name[] = "D:\\3e635-cvgs.jpg";

	Mat3b img_1 = imread(img_1_name);
	//imshow("Test", img_1);
	//waitKey(6000);
	if (!img_1.data) {
		printf("Image not loaded.");
		exit(255);
	}
	imshow("src", img_1);
	waitKey(2000);

	// Преобразуем RGB в HSV средствами opencv
	Mat3b img_1_hsv;
	cvtColor(img_1, img_1_hsv, CV_BGR2HSV);
	imshow("bgr2hsv_cvtcolor", img_1_hsv);
	waitKey(5000);

	// Преобразуем RGB в HSV средствами своей реализации конвертации
	Mat3b img = img_1.clone();   
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			Vec3b rgb_pixel = img_1(i, j);
			float h, s, v;
			rgb_to_hsv(rgb_pixel[0], rgb_pixel[1], rgb_pixel[2], &h, &s, &v);
			Vec3b hsv_pixel = Vec3b((uchar)h, (uchar)(s*255), (uchar)(v*255));
			img(i, j) = hsv_pixel;
		}
	}
	//imshow("bgr2hsv1", img);
	//waitKey(5000);

	// Применяем фильтр яркости для rgb
	Mat img_bright = img_1.clone();
	bright(img_1, img_bright, 50);
	//imshow("img_1_bright", img_bright);
	//waitKey(5000);

	printf("\nMetrics of source rgb file and bright version\n");
	compute_quality_metrics(img_1, img_bright, 10);

	// Применяем фильтр яркости для hsv
	Mat img_hsv_bright = img_1_hsv.clone();
	bright_hsv(img_1_hsv, img_hsv_bright, -50);
	imshow("img_hsv_bright", img_hsv_bright);
	waitKey(5000);

	printf("\nMetrics of source hsv file and bright version\n");
	compute_quality_metrics(img_1_hsv, img_hsv_bright, 10);
	return;

	static int Bs = 0, Gs = 0, Rs = 0;
	namedWindow("colourCtrl");
	for (;;)
	{
		//cap>>src;
		createTrackbar("B", "colourCtrl", &Bs, 255);
		createTrackbar("G", "colourCtrl", &Gs, 255);
		createTrackbar("R", "colourCtrl", &Rs, 255);
		Mat refRGB(500, 500, CV_8UC3, Scalar(Bs, Gs, Rs));
		src = refRGB;
		cvtColor(src, hsv, CV_BGR2HSV);
		imshow(RGB_window, src);
		imshow(HSV_window, hsv);
		setMouseCallback(RGB_window, onMouse, 0);
		setMouseCallback(HSV_window, onMouse, 0);
		char c = waitKey(10);
		if (c == 'b')
		{
			break;
		}
	}
	return;
}

Mat apply_constant_noise(Mat img, std::vector<Pixel_Data> bad_pixels)
{
    
    Mat img_out = img.clone();
    for (int i = 0; i < bad_pixels.size(); i++) {
        Pixel_Data bad_pixel = bad_pixels[i];
        if (bad_pixel.x >= 0 && bad_pixel.x < img_out.cols && bad_pixel.y >= 0 && bad_pixel.y < img_out.rows) {
          //  printf("rows: %d, cols: %d, x: %d, y: %d\n", img_out.rows, img_out.cols, bad_pixel.x, bad_pixel.y);
            img_out.at<Pixel>(bad_pixel.y, bad_pixel.x) = 
                Pixel(bad_pixels[i].pixel.x, bad_pixels[i].pixel.y, bad_pixels[i].pixel.z);
        }
    }
    return img_out;

    for (int i = 0; i < bad_pixels.size(); i++) {
        Pixel_Data bad_pixel = bad_pixels[i];
        if (bad_pixel.x >= 0 && bad_pixel.x < img_out.cols && bad_pixel.y >= 0 && bad_pixel.y < img_out.rows) {
          //  printf("rows: %d, cols: %d, x: %d, y: %d\n", img_out.rows, img_out.cols, bad_pixel.x, bad_pixel.y);
            img_out.at<Pixel>(10, 10) = Pixel(bad_pixel.pixel.x, bad_pixel.pixel.y, bad_pixel.pixel.z);
            Pixel &p= img.at<Pixel>(314, 437);
            img_out.at<Pixel>(314, 437) = Pixel(bad_pixel.pixel.x, bad_pixel.pixel.y, bad_pixel.pixel.z);
            img_out.at<Pixel>(bad_pixel.x, bad_pixel.y) = Pixel(255, 255, 255);
            img_out.at<Pixel>(bad_pixel.x, bad_pixel.y) = Pixel(bad_pixel.pixel.x, bad_pixel.pixel.y, bad_pixel.pixel.z);
        }
    }
    return img_out;
}

void part_four()
{
    char img_1_name[] = "D:\\3e635.jpg";
    char img_1_gs_name[] = "D:\\3e635-gs.jpg";
    char img_1_cvgs_name[] = "D:\\3e635-cvgs.jpg";
    char img_2_name[] = "D:\\3e635a.jpg";
    Mat img_1 = imread(img_1_name, cv::IMREAD_COLOR);
    printf("TYPE_MASK: %d, channels: %d)\n", img_1.TYPE_MASK, img_1.channels());
    imshow("Test", img_1);
    //waitKey(6000);
    if (!img_1.data) {
        printf("Image not loaded.");
        return;
    }

    std::vector<Pixel_Data> bad_pixels(100);

    std::mt19937 gen(uint32_t(time(0)));
    std::uniform_int_distribution<uint> uid_rgb(0, +255);
    std::uniform_int_distribution<int> uid_x(0, img_1.cols-1);
    std::uniform_int_distribution<int> uid_y(0, img_1.rows-1);
    
    //Создаем случайный список битых пикселей
    for (int i = 0; i < bad_pixels.size(); i++) {
        bad_pixels[i].x = uid_x(gen);
        bad_pixels[i].y = uid_y(gen);
        bad_pixels[i].pixel.x = 0; // uid_rgb(gen);
        bad_pixels[i].pixel.y = 0; // uid_rgb(gen);
        bad_pixels[i].pixel.z = 255; // uid_rgb(gen);
        //printf("Pixel[%d] = (x: %d, y:%d, pixel(r,g,b) = (%d, %d, %d)\n", i, bad_pixels[i].x, bad_pixels[i].y,
           // bad_pixels[i].pixel.x, bad_pixels[i].pixel.y, bad_pixels[i].pixel.z);
    }
    //printf("cols: %d, rows: %d\n", img_1.cols, img_1.rows);

    Mat img_with_noise = apply_constant_noise(img_1, bad_pixels);
    imshow("Constant noise", img_with_noise);
    waitKey(10);
    
    printf("\n\nQuality metrics for image with noise\n");
    compute_quality_metrics(img_1, img_with_noise, 10);

    auto gf = new GaussianFilter(1, 2.0);
    
    auto start = high_resolution_clock::now();
    Mat img_after_gf = gf->processImage(img_with_noise);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    imshow("Noised image after gf", img_after_gf);
    waitKey(10);
    printf("\n\nQuality metrics for image with filtered noise [GaussianFilter (%d ms)]\n", int(duration.count()));
    compute_quality_metrics(img_1, img_after_gf, 10);
    
    auto mf = new MedianFilter();

    start = high_resolution_clock::now();
    Mat img_after_mf = mf->processImage(img_with_noise);
    imshow("Noised image after mf", img_after_mf);
    stop = high_resolution_clock::now();
    duration = duration_cast<std::chrono::milliseconds>(stop - start);
    waitKey(5000);
    printf("\n\nQuality metrics for image with filtered noise [MedianFilter (%d ms)]\n", int(duration.count()));
    compute_quality_metrics(img_1, img_after_mf, 10);

    //printf("avg1=%f, dev1=%f, avg2=%f, dev2=%f\n\n", img_1_mean.val[0], img_1_dev.val[0], img_2_mean.val[0], img_2_dev.val[0]);
    //Mat img_with_cv_noise = imnoise(img_1, 'salt & pepper', 0.02);
    
    //imshow("Constant noise", img_with_noise);
    //waitKey(10);

    Mat img_after_opencv_gf;
    int kernel_size = 3;
    double sigma = 2.0;
    start = high_resolution_clock::now();
    GaussianBlur(img_1, img_after_opencv_gf, Size(kernel_size, kernel_size), sigma);
    stop = high_resolution_clock::now();
    duration = duration_cast<std::chrono::milliseconds>(stop - start); 
    imshow("Noised image after opencv gf", img_after_opencv_gf);
    printf("\n\nQuality metrics for image with filtered noise [GaussianFilter (%d ms)]\n", int(duration.count()));
    compute_quality_metrics(img_after_gf, img_after_opencv_gf, 10);
    waitKey(5000);

    Mat img_after_opencv_mf;
    start = high_resolution_clock::now();
    medianBlur(img_1, img_after_opencv_mf, 3);
    stop = high_resolution_clock::now();
    duration = duration_cast<std::chrono::milliseconds>(stop - start);
    imshow("Noised image after opencv mf", img_after_opencv_mf);
    printf("\n\nQuality metrics for image with filtered noise [MedianFilter (%d ms)]\n", int(duration.count()));
    compute_quality_metrics(img_after_mf, img_after_opencv_mf, 10);
    waitKey(5000);


    waitKey(0);


}


void part_five() {
    //char img_1_name[] = "D:\\3e635.jpg";
    char img_1_name[] = "D:\\coins.jpg";
    char img_1_gs_name[] = "D:\\3e635-gs.jpg";

    Mat img_1 = imread(img_1_name, cv::IMREAD_COLOR);
    //printf("TYPE_MASK: %d, channels: %d)\n", img_1.TYPE_MASK, img_1.channels());
    imshow("Source color image", img_1);
    //waitKey(1000);
    if (!img_1.data) {
        printf("Image not loaded.");
    }

    Mat img_2 = img_1.clone();
    Mat contours;
    Canny(img_2, contours, 100, 200);
    imshow("OCV Canny contours", contours);
    waitKey(1000);

    Mat img_my_canny = img_1.clone();
    Mat my_contours;
    canny(img_2, my_contours, 10, 70);
    imshow("My Canny contours 10, 100", my_contours);

    cv::Mat img_added = img_1.clone();
    std::vector<cv::Vec3b> circles(0);
    cv::Mat img_circles = hough_circle_detection(contours, circles, 20.0f, 20, 20, 20);
    for (size_t i = 0; i < circles.size(); i++) {
        circle(img_added, cv::Point(circles[i][1], circles[i][0]), circles[i][2], cv::Scalar(255));
    }
    imshow("My added circle det", img_added);
    
    //cv::Mat img_blending = img_1.clone();
    //for (size_t i = 0; i < img_blending.rows; i++) {
    //    for (size_t j = 0; j < img_blending.cols; j++) {
    //        cv::Vec3b &dst = img_blending.at<cv::Vec3b>(i, j);
    //        //uchar &add = img_circles.at<uchar>(i, j);
    //        //if (add) {
    //        //    dst[0] = 0;
    //        //    dst[1] = 255;
    //        //    dst[2] = 0;
    //        //}
    //    }
    //}
    //imshow("My blending", img_blending);

    imshow("img_circles", img_circles);
    print_detection_stat(circles);

    compute_quality_metrics(contours, my_contours, 10); 
    waitKey(30000);
}

