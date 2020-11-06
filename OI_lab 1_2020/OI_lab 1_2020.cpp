//// OI_lab 1_2020.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
////

#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <random>

#include "qmetrics.h"
#include "rgbhsv.h"
#include "OI_lab 1_2020.h"

#define CLAMP(_v, _min, _max)\
	(_v < _min ? _min: _v > _max ? _max: _v)

using namespace cv;
using namespace std;
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
    part_four();

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

	//for (int row = 0; row < img_1.rows; row++) {
	//	for (int col = 0; col < img_1.cols; col++) {
	//		Vec3b pixel_color(img_1.data[row, col]);
	//		img_1.data[row, col] = Vec3b(pixel_color[0], pixel_color[1], pixel_color[2]);
	//	}
	//}

	//IplImage img_1_array = img_1;
	//for (int row = 0; row < img_1_array.width; row++) {
	//	for (int col = 0; col < img_1_array.width; col++) {
	//		img_1_array.imageData[col][row]
	//	}
	//}
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
	//img_1 /= 255;
	//imshow("Test", img_1);
	//waitKey(6000);
	if (!img_1.data) {
		printf("Image not loaded.");
		exit(255);
	}
	imshow("src", img_1);
	waitKey(2000);

	// Преобразуем RGB в HSV средствами opencv
	//Mat img_1_hsv = img_1.clone();
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

	//Mat3b img_1_hsv1 = img_1.clone();
	//for (int row = 0; row < img_1.rows; row++) {
	//	for (int col = 0; col < img_1.cols; col++) {
	//		Vec3b pixel = Vec3b(0, 0, 0);
	//		Vec3b pixel_color = img_1.at<Vec3b>(row, col);
	//		//img_1.at<Vec3b>(row, col) = pixel;
	//		//img_1_hsv1(row, col) = pixel;
	//	}
	//}
	//imshow("bgr2hsv1_cvtcolor", img_1_hsv1);
	//waitKey(5000);

	//typedef cv::Point3_<uint8_t> Pixel;
	//Mat3f img_1_hsv1 = img_1;
	//img_1_hsv1.forEach<Vec3f>([](Vec3f &p, const int * position) -> void {
	//	double value = (p[0] + p[1] + p[2]) / 3;
	//	p[0] = value;
	//	p[1] = value;
	//	p[2] = value;
	//});
	//
	//
	//for (int row = 0; row < img_1.rows; row++) {
	//	for (int col = 0; col < img_1.cols; col++) {
	//		Vec3f pixel = Vec3f(0, 0, 0);
	//		Vec3b pixel_color(img_1.data[row, col]);
	//		img_1_hsv1(row, col) = pixel;
	//	}
	//}
	//imshow("bgr2hsv1_cvtcolor", img_1_hsv1);
	//waitKey(5000);
	
	return;

	static int Bs = 0, Gs = 0, Rs = 0;
	namedWindow("colourCtrl");
	//src = imread("bgr.png",1);
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
            printf("rows: %d, cols: %d, x: %d, y: %d\n", img_out.rows, img_out.cols, bad_pixel.x, bad_pixel.y);
            img_out.at<Pixel>(bad_pixel.y, bad_pixel.x) = 
                Pixel(bad_pixels[i].pixel.x, bad_pixels[i].pixel.y, bad_pixels[i].pixel.z);
        }
    }
    return img_out;

    //for (int i = 0; i < img.rows; i++) {
    //    for (int j = 0; j < img.cols; j++) {
    //        Vec3b rgb_pixel = img_1(i, j);
    //        float h, s, v;
    //        rgb_to_hsv(rgb_pixel[0], rgb_pixel[1], rgb_pixel[2], &h, &s, &v);
    //        Vec3b hsv_pixel = Vec3b((uchar)h, (uchar)(s * 255), (uchar)(v * 255));
    //        img(i, j) = hsv_pixel;
    //    }
    //}
    //
    
    //Mat img_out = img.clone();

    for (int i = 0; i < bad_pixels.size(); i++) {
        Pixel_Data bad_pixel = bad_pixels[i];
        if (bad_pixel.x >= 0 && bad_pixel.x < img_out.cols && bad_pixel.y >= 0 && bad_pixel.y < img_out.rows) {
            printf("rows: %d, cols: %d, x: %d, y: %d\n", img_out.rows, img_out.cols, bad_pixel.x, bad_pixel.y);
            //Pixel &p = img_out.at<Pixel>(bad_pixel.y, bad_pixel.x);
            //Pixel &p = img_out.at<Pixel>(10, 10);
            img_out.at<Pixel>(10, 10) = Pixel(bad_pixel.pixel.x, bad_pixel.pixel.y, bad_pixel.pixel.z);
            //img(314, 417) = Pixel(bad_pixel.pixel.x, bad_pixel.pixel.y, bad_pixel.pixel.z);
            Pixel &p= img.at<Pixel>(314, 437);
            img_out.at<Pixel>(314, 437) = Pixel(bad_pixel.pixel.x, bad_pixel.pixel.y, bad_pixel.pixel.z);
            img_out.at<Pixel>(bad_pixel.x, bad_pixel.y) = Pixel(255, 255, 255);
            img_out.at<Pixel>(bad_pixel.x, bad_pixel.y) = Pixel(bad_pixel.pixel.x, bad_pixel.pixel.y, bad_pixel.pixel.z);
/*            img_out.at<Vec3b>(10, 10) = Vec3b(bad_pixel.pixel.x, bad_pixel.pixel.y, bad_pixel.pixel.z);
            img_out.at<Vec3b>(314, 437) = Vec3b(bad_pixel.pixel.x, bad_pixel.pixel.y, bad_pixel.pixel.z);
            img_out.at<Vec3b>(bad_pixel.x, bad_pixel.y) = Vec3b(255, 255, 255);
            img_out.at<Vec3b>(bad_pixel.x, bad_pixel.y) = Vec3b(bad_pixel.pixel.x, bad_pixel.pixel.y, bad_pixel.pixel.z);
 */       }
    }
    



    //cv::Point3f &p = img.at<Point3f>(10, 10);
    //for (int i = 0; i < bad_pixels.size(); i++) {
    //    Pixel_Data bad_pixel = bad_pixels[i];
    //    if (bad_pixel.x >= 0 && bad_pixel.x < img_out.cols && bad_pixel.y >= 0 && bad_pixel.y < img_out.rows) {
    //        printf("rows: %d, cols: %d, x: %d, y: %d\n", img_out.rows, img_out.cols, bad_pixel.x, bad_pixel.y);
    //        //Pixel &p = img_out.at<Pixel>(bad_pixel.y, bad_pixel.x);
    //        Pixel &p = img_out.at<Pixel>(10, 10);
    //        //p.x = bad_pixel.pixel.x;
    //        //p.y = bad_pixel.pixel.y;
    //        //p.z = bad_pixel.pixel.z;
    //    }
    //}
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
        printf("Pixel[%d] = (x: %d, y:%d, pixel(r,g,b) = (%d, %d, %d)\n", i, bad_pixels[i].x, bad_pixels[i].y,
            bad_pixels[i].pixel.x, bad_pixels[i].pixel.y, bad_pixels[i].pixel.z);
    }
    printf("cols: %d, rows: %d\n", img_1.cols, img_1.rows);

    Mat img_2 = apply_constant_noise(img_1, bad_pixels);
    imshow("Constant noise", img_2);
    waitKey(16000);
   

    //printf("avg1=%f, dev1=%f, avg2=%f, dev2=%f\n\n", img_1_mean.val[0], img_1_dev.val[0], img_2_mean.val[0], img_2_dev.val[0]);

    waitKey(0);

    compute_quality_metrics(img_1, img_2, 10);
}


