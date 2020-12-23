#include <cmath>
#include <iostream>
#include "hough.h"


int approximate(float x, int step)
{
    int temp = round(x / step);
    return temp * step;
}


cv::Mat hough_circle_detection(cv::Mat img, std::vector<cv::Vec3b> &circles, float threshold, 
    int neighbour_size, int rad_step, int center_step)
{
    cv::Mat img_edge, img_circle;
    Canny(img, img_edge, 75, 200, 3, true);
    //return img_edge;
    //circles = std::vector<cv::Vec3b>(0);
    int max_r = std::min(img_edge.rows, img_edge.cols) / 2;
    img_circle = cv::Mat::zeros(img.rows, img.cols, CV_8U);

    for (int r = rad_step; r < max_r; r += rad_step)
    {
        cv::Mat circle_centers = cv::Mat::zeros(img.rows, img.cols, CV_32F);
        for (int y = 0; y < img_edge.rows; y++)
            for (int x = 0; x < img_edge.cols; x++)
            {
                if (img_edge.at<uint8_t>(y, x) < 255)
                    continue;
                for (int a = rad_step; a < img_edge.rows - rad_step; a += center_step)
                {
                    if (r * r - (y - a) * (y - a) < 0.0)
                        continue;
                    int b1 = approximate(x - sqrt(r * r - (y - a) * (y - a)), center_step);
                    int b2 = approximate(x + sqrt(r * r - (y - a) * (y - a)), center_step);
                    if (b1 >= rad_step && b1 < img_edge.cols - rad_step)
                        circle_centers.at<float>(a, b1) += 1.0;
                    if (b2 >= rad_step && b2 < img_edge.cols - rad_step)
                        circle_centers.at<float>(a, b2) += 1.0;
                }
            }
        for (int a = rad_step; a < img_edge.rows - rad_step; a += center_step)
            for (int b = rad_step; b < img_edge.cols - rad_step; b += center_step)
            {
                if (circle_centers.at<float>(a, b) > threshold)
                {
                    bool local_max = true;
                    for (int i = a - neighbour_size * center_step; i <= a + neighbour_size * center_step; i += center_step)
                        if (local_max && i >= rad_step && i < img_edge.rows - rad_step)
                            for (int j = b - neighbour_size * center_step; j <= b + neighbour_size * center_step; j += center_step)
                                if (j >= rad_step && j < img_edge.cols - rad_step)
                                    if (circle_centers.at<float>(a, b) < circle_centers.at<float>(i, j))
                                    {
                                        local_max = false;
                                        break;
                                    }

                    if (local_max)
                    {
                        //std::cout << circle_centers.at<float>(a, b) << " ";
                        draw_circle(img_circle, a, b, r);
                        circles.push_back(cv::Vec3b(a, b, r));
                    }
                }
            }
    }
    return img_circle;
}

cv::Mat hough_ellipse_detection(cv::Mat img, std::vector<cv::Vec3b> &circles, float threshold,
    int neighbour_size, int rad_step, int center_step)
{
    cv::Mat img_edge, img_ellipse;
    Canny(img, img_edge, 75, 200, 3, true);
    ////return img_edge;
    int max_r = std::min(img_edge.rows, img_edge.cols) / 2;
    img_ellipse = cv::Mat::zeros(img.rows, img.cols, CV_8U);

    std::vector< std::vector<cv::Mat> > accumelator;
    for (int r1 = rad_step; r1 < max_r; r1 += rad_step)
    {
        std::vector<cv::Mat> temp;
        for (int r2 = rad_step; r2 < max_r; r2 += rad_step)
        {
            cv::Mat ellipse_centers = cv::Mat::zeros(img.rows, img.cols, CV_32F);
            temp.push_back(ellipse_centers);
        }
        std::cout<<temp.size()<<" "<<max_r / rad_step<< "\n";
        accumelator.push_back(temp);
    }

    for (int r1 = rad_step; r1 < max_r; r1 += rad_step)
    {
        int i = r1 / rad_step;
        for (int r2 = rad_step; r2 < max_r; r2 += rad_step)
        {
            int j = r2 / rad_step;
            for (int y = 0; y < img_edge.rows; y++)
                for (int x = 0; x < img_edge.cols; x++)
                {
                    if (img_edge.at<uint8_t>(y, x) < 255)
                        continue;
                    for (int a = rad_step; a < img_edge.rows - rad_step; a += center_step)
                    {
                        float d = r2 * r2 * (1 - (y - a) * (y - a) * 1.0 / (r1 * r1));
                        if (d < 0.0)
                            continue;
                        int b1 = approximate(x - sqrt(d), center_step);
                        int b2 = approximate(x + sqrt(d), center_step);
                        if (b1 >= rad_step && b1 < img_edge.cols - rad_step)
                            accumelator[i - 1][j - 1].at<float>(a, b1) += 1.0;
                        if (b2 >= rad_step && b2 < img_edge.cols - rad_step)
                            accumelator[i - 1][j - 1].at<float>(a, b2) += 1.0;
                    }
                }
        }
    }

    for (int r1 = rad_step; r1 < max_r; r1 += rad_step)
    {
        int i = r1 / rad_step;
        for (int r2 = rad_step; r2 < max_r; r2 += rad_step)
        {
            int j = r2 / rad_step;
            for (int a = rad_step; a < img_edge.rows - rad_step; a += center_step)
                for (int b = rad_step; b < img_edge.cols - rad_step; b += center_step)
                {

                    if (accumelator[i - 1][j - 1].at<float>(a, b) <= threshold)
                        continue;
                    bool local_max = true;
                    for (int d1 = i - neighbour_size; local_max && d1 <= i + neighbour_size && d1 < max_r / rad_step; d1++)
                    {
                        if (d1 < 1)
                            continue;
                        for (int d2 = j - neighbour_size; local_max && d2 <= j + neighbour_size && d2 < max_r / rad_step; d2++)
                        {
                            if (d2 < 1)
                                continue;
                            for (int d3 = a - neighbour_size * center_step; local_max && d3 <= a + neighbour_size * center_step && d3 < img_edge.rows - rad_step; d3 += center_step)
                            {
                                if (d3 < rad_step)
                                    continue;
                                for (int d4 = b - neighbour_size * center_step; local_max && d4 <= b + neighbour_size * center_step && d4 < img_edge.rows - rad_step; d4 += center_step)
                                {
                                    if (d4 < rad_step)
                                        continue;
                                    if (accumelator[i - 1][j - 1].at<float>(a, b) < accumelator[d1 - 1][d2 - 1].at<float>(d3, d4))
                                        local_max = false;
                                }
                            }
                        }
                    }

                    if (local_max)
                    {
                        ellipse(img_ellipse, cv::Point(b, a), cv::Size(r2, r1), 0.0, 0.0, 360, cv::Scalar(255, 255, 255));
                    }

                }
        }
    }
    return img_ellipse;
}


void draw_circle(cv::Mat & img, int a, int b, int r) {
    circle(img, cv::Point(b, a), r, cv::Scalar(255));
   //! std::cout << a << " " << b << " " << r << std::endl;
}

void print_detection_stat(std::vector<cv::Vec3b> &circles) {
    std::cout << "Statistics:\n";
    if (circles.size() == 0) {
        std::cout << "No circles found\n";
    }
    else {
        int max_radius = circles[0][2];
        int min_radius = circles[0][2];
        int average_radius = 0;
        int sum_radius = 0;

        for (int i = 0; i < circles.size(); i++) {
            if (circles[i][2] > max_radius)
                max_radius = circles[i][2];
            if (circles[i][2] < min_radius)
                min_radius = circles[i][2];
            sum_radius += circles[i][2];
        }
        average_radius = sum_radius / circles.size();
        std::cout << "Maximum radius:" << max_radius << "\n";
        std::cout << "Minimum radius:" << min_radius << "\n";
        std::cout << "Average radius:" << average_radius << "\n";
        std::cout << "Number of circles:" << circles.size() << "\n";
    }
        
    
}