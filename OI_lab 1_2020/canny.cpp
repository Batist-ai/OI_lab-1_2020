#include "canny.h"

void canny(cv::Mat &src_image, cv::Mat &contours, int low, int high)
{
    cv::Mat gs_image; // Оттенки серого
    cv::Mat gs_filtered_image; // Градиент
    cv::Mat sobel_filtered_image; // Фильтр Собеля
    cv::Mat angles; // 
    cv::Mat max_supressed_image; // Подавление не максимальных значений
    cv::Mat result_image; 

    // Оттенки серого
    //cvtColor(src_image, gs_image, CV_BGR2GRAY);
    gs_image = convert_to_grayscale(src_image);

    // Фильтр Гаусса
    cv::GaussianBlur(gs_image, gs_filtered_image, cv::Size(3, 3), 1.0);

    // Фильтр Собеля
    sobel_filtered_image = sobel(gs_filtered_image, angles);
    //cv::imshow("sobel_filtered_image", sobel_filtered_image);
    //cv::imshow("angles", angles);
    //cv::waitKey(5000);


    // Подавляем НЕ максимальные значения
    max_supressed_image = non_max_suppression(sobel_filtered_image, angles);

    
    result_image = double_threshold(max_supressed_image, low, high);

    contours = result_image.clone();
}


cv::Mat convert_to_grayscale(cv::Mat image)
{
    cv::Mat result_image = cv::Mat(image.rows, image.cols, CV_8UC1); // 1 канал
    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
        {
            int b = image.at<cv::Vec3b>(i, j)[0];
            int g = image.at<cv::Vec3b>(i, j)[1];
            int r = image.at<cv::Vec3b>(i, j)[2];

            //double newValue = (r * 0.2126 + g * 0.7152 + b * 0.0722);
            double newValue = (r * 0.299 + g * 0.587 + b * 0.114);
            result_image.at<uchar>(i, j) = newValue;

        }
    return result_image;
}

cv::Mat sobel(cv::Mat image, cv::Mat &angles)
{
    // Собель_X
    double x1[] = { -1.0, 0, 1.0 };
    double x2[] = { -2.0, 0, 2.0 };
    double x3[] = { -1.0, 0, 1.0 };

    std::vector<std::vector<double>> xFilter(3);
    xFilter[0].assign(x1, x1 + 3);
    xFilter[1].assign(x2, x2 + 3);
    xFilter[2].assign(x3, x3 + 3);

    // Собель_Y
    double y1[] = { 1.0, 2.0, 1.0 };
    double y2[] = { 0, 0, 0 };
    double y3[] = { -1.0, -2.0, -1.0 };

    std::vector<std::vector<double>> yFilter(3);
    yFilter[0].assign(y1, y1 + 3);
    yFilter[1].assign(y2, y2 + 3);
    yFilter[2].assign(y3, y3 + 3);

    // Макс размер
    int size = (int)xFilter.size() / 2;

    cv::Mat filtered_image = cv::Mat(image.rows - 2 * size, image.cols - 2 * size, CV_8UC1);

    angles = cv::Mat(image.rows - 2 * size, image.cols - 2 * size, CV_32FC1); // Angle

    for (int i = size; i < image.rows - size; i++)
    {
        for (int j = size; j < image.cols - size; j++)
        {
            double sumx = 0;
            double sumy = 0;

            for (int x = 0; x < xFilter.size(); x++)
                for (int y = 0; y < xFilter.size(); y++)
                {
                    sumx += xFilter[x][y] * (double)(image.at<uchar>(i + x - size, j + y - size)); // Собель_Х значение
                    sumy += yFilter[x][y] * (double)(image.at<uchar>(i + x - size, j + y - size)); // Собель_Y значение 
                }
            double sumxsq = sumx * sumx;
            double sumysq = sumy * sumy;

            double sq2 = sqrt(sumxsq + sumysq);

            if (sq2 > 255) // UnsignedChar фикс
                sq2 = 255;
            filtered_image.at<uchar>(i - size, j - size) = sq2;

            if (sumx == 0) 
                angles.at<float>(i - size, j - size) = 90;
            else
                angles.at<float>(i - size, j - size) = atan(sumy / sumx);
        }
    }
    return filtered_image;
}

cv::Mat non_max_suppression(cv::Mat image, cv::Mat angles) {
    cv::Mat supressed_image = cv::Mat(image.rows - 2, image.cols - 2, CV_8UC1);
    for (int i = 1; i < image.rows - 1; i++) {
        for (int j = 1; j < image.cols - 1; j++) {
            float Tangent = angles.at<float>(i, j);

            supressed_image.at<uchar>(i - 1, j - 1) = image.at<uchar>(i, j);
            // Горизонтаьно
            if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
            {
                if ((image.at<uchar>(i, j) < image.at<uchar>(i, j + 1)) || (image.at<uchar>(i, j) < image.at<uchar>(i, j - 1)))
                    supressed_image.at<uchar>(i - 1, j - 1) = 0;
            }
            // Вертикально
            if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
            {
                if ((image.at<uchar>(i, j) < image.at<uchar>(i + 1, j)) || (image.at<uchar>(i, j) < image.at<uchar>(i - 1, j)))
                    supressed_image.at<uchar>(i - 1, j - 1) = 0;
            }

            // -45 градусов
            if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
            {
                if ((image.at<uchar>(i, j) < image.at<uchar>(i - 1, j + 1)) || (image.at<uchar>(i, j) < image.at<uchar>(i + 1, j - 1)))
                    supressed_image.at<uchar>(i - 1, j - 1) = 0;
            }

            // 45 градусов
            if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
            {
                if ((image.at<uchar>(i, j) < image.at<uchar>(i + 1, j + 1)) || (image.at<uchar>(i, j) < image.at<uchar>(i - 1, j - 1)))
                    supressed_image.at<uchar>(i - 1, j - 1) = 0;
            }
        }
    }
    return supressed_image;
}

cv::Mat double_threshold(cv::Mat image, int low, int high)
{
    if (low > 255)
        low = 255;
    if (high > 255)
        high = 255;

    cv::Mat result_image = cv::Mat(image.rows, image.cols, image.type());

    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            result_image.at<uchar>(i, j) = image.at<uchar>(i, j);
            if (result_image.at<uchar>(i, j) > high)
                result_image.at<uchar>(i, j) = 255;
            else if (result_image.at<uchar>(i, j) < low)
                result_image.at<uchar>(i, j) = 0;
            else
            {
                bool anyHigh = false;
                bool anyBetween = false;
                for (int x = i - 1; x < i + 2; x++)
                {
                    for (int y = j - 1; y < j + 2; y++)
                    {
                        if (x <= 0 || y <= 0 || result_image.rows || y > result_image.cols) // За границами
                            continue;
                        else
                        {
                            if (result_image.at<uchar>(x, y) > high)
                            {
                                result_image.at<uchar>(i, j) = 255;
                                anyHigh = true;
                                break;
                            }
                            else if (result_image.at<uchar>(x, y) <= high && result_image.at<uchar>(x, y) >= low)
                                anyBetween = true;
                        }
                    }
                    if (anyHigh)
                        break;
                }
                if (!anyHigh && anyBetween)
                    for (int x = i - 2; x < i + 3; x++)
                    {
                        for (int y = j - 1; y < j + 3; y++)
                        {
                            if (x < 0 || y < 0 || x > result_image.rows || y > result_image.cols) // За границами
                                continue;
                            else
                            {
                                if (result_image.at<uchar>(x, y) > high)
                                {
                                    result_image.at<uchar>(i, j) = 255;
                                    anyHigh = true;
                                    break;
                                }
                            }
                        }
                        if (anyHigh)
                            break;
                    }
                if (!anyHigh)
                    result_image.at<uchar>(i, j) = 0;
            }
        }
    }
    return result_image;
}