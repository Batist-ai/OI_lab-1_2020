#include <math.h>
#include "pixel.h"
#include "filters.h"

using namespace cv;

// Filter

//Pixel Filter::calculateNewPixel(Mat sourceImage, int x, int y) {
//    return Pixel(sourceImage.at<Pixel>(x, y));
//}

Mat Filter::preprocessImage(Mat sourceImage) {
    return sourceImage;
}

Filter::Filter() {}

int Filter::Clamp(int value, int min, int max) {
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

Mat Filter::processImage(Mat sourceImage) {

    printf("Filter::processImage()\n");
    Mat preprocessedImage = Mat(preprocessImage(sourceImage));
    Mat resultImage = preprocessedImage.clone();
    for (int i = 0; i < preprocessedImage.rows; i++)
    {
        for (int j = 0; j < preprocessedImage.cols; j++)
        {
            resultImage.at<Pixel>(i, j) = calculateNewPixel(preprocessedImage, i, j);
        }
    }

    return resultImage;
}


// MatrixFilter

MatrixFilter::MatrixFilter(): MatrixFilter(FILTERS_MF_DEFAULT_RADIUS) { }

MatrixFilter::MatrixFilter(std::vector<float> kernel, size_t radius) {
    assert(kernel.size() > (radius + 1) * (radius + 1));
    m_radius = radius;
    m_kernel = kernel;
    m_cols = radius * 2 + 1;
}

MatrixFilter::MatrixFilter(size_t radius) {
    m_radius = radius;
    m_cols = radius * 2 + 1;
    m_kernel = std::vector<float>(m_cols * m_cols);
}

Pixel MatrixFilter::calculateNewPixel(Mat sourceImage, int y, int x) {
    int radiusX = int(m_radius);
    int radiusY = int(m_radius);

    float resultR = 0;
    float resultG = 0;
    float resultB = 0;

    for (int i = -radiusY; i <= radiusY; i++) {
        for (int j = -radiusX; j <= radiusX; j++) {
            int idX = Clamp(x + j, 0, sourceImage.cols - 1);
            int idY = Clamp(y + i, 0, sourceImage.rows - 1);
            Pixel &p = sourceImage.at<Pixel>(idY, idX);
            int idK = (j + radiusX) * m_cols + i + radiusY;
            resultR += p.x * m_kernel[idK];
            resultG += p.y * m_kernel[idK];
            resultB += p.z * m_kernel[idK];
        }
    }
    return Pixel(
        Clamp((int)resultR, 0, 255),
        Clamp((int)resultG, 0, 255),
        Clamp((int)resultB, 0, 255)
    );
}

void MatrixFilter::print_kernel(std::string caption) {
    printf("%s: {\n", caption);
    int cols = int(m_cols);
    for (int i = 0; i < cols; i++) {
        printf("   ");
        for (int j = 0; j < cols; j++) {
            printf("%5.3f, ", m_kernel[i * cols + j]);
        }
        printf("\n");
    }
    printf("}\n");
}


// GaussianFilter

GaussianFilter::GaussianFilter(): GaussianFilter(FILTERS_MF_DEFAULT_RADIUS, FILTERS_GF_DEFAULT_SIGMA) {}
GaussianFilter::GaussianFilter(size_t radius, float sigma): MatrixFilter(radius) {
    assert(sigma != 0.0);
    m_sigma = sigma;
    createGaussianKernel();
}

void GaussianFilter::createGaussianKernel()
{
    float norm = 0;
    int cols = int(m_cols);
    int radius = int(m_radius);

    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            int idK = (i + radius) * cols + j + radius;
            printf("idk: %d\n", idK);
            m_kernel[idK] = (float)(exp(-(i * i + j * j) / (2 * m_sigma * m_sigma)));
            norm += m_kernel[idK];
        }
    }
    print_kernel("Print initialized kernel");

    for (size_t i = 0; i < cols; i++)
        for (size_t j = 0; j < cols; j++)
            m_kernel[i * cols + j] /= norm;

    print_kernel("Print normilized kernel");
}


// MedianFilter

MedianFilter::MedianFilter(): MatrixFilter(1) {
    createKernel();
}

void MedianFilter::createKernel()
{
    int cols = int(m_cols);
    int radius = int(m_radius);

    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            int idK = (i + radius) * cols + j + radius;
            m_kernel[idK] = 1.0;
        }
    }
    print_kernel("Print initialized kernel");
}

Pixel MedianFilter::calculateNewPixel(Mat sourceImage, int y, int x) {
    int radiusX = int(m_radius);
    int radiusY = int(m_radius);

    for (int i = -radiusY; i <= radiusY; i++) {
        for (int j = -radiusX; j <= radiusX; j++) {
            int idX = Clamp(x + j, 0, sourceImage.cols - 1);
            int idY = Clamp(y + i, 0, sourceImage.rows - 1);
            Pixel &p = sourceImage.at<Pixel>(idY, idX);
            int idK = (j + radiusX) * m_cols + i + radiusY;
            m_kernel[idK] = (0.2126 * p.x + 0.7152 * p.y + 0.0722 * p.z);
        }
    }
    sort(m_kernel.begin(), m_kernel.end());
    int median_Y = Clamp(m_kernel[m_kernel.size() / 2], 0, 255);
    Pixel &p = sourceImage.at<Pixel>(y, x);
    int br = 255;
    return Pixel(
        p.x * median_Y / br,
        p.y * median_Y / br,
        p.z * median_Y / br
    );
}


// ProcessingTask

Mat ProcessingTask::get_image() {
    return m_image;
}

void ProcessingTask::set_image(Mat image) {
    m_image = image;
}

std::vector<std::string> ProcessingTask::filters() {
    return m_filters; 
}

void ProcessingTask::set_filters(std::vector<std::string> filters) {
    m_filters = filters;
}

ProcessingTask::ProcessingTask(Mat image, std::vector<std::string> filters) {
    m_filters = filters;
    m_image = image;
}

Filter *ProcessingTask::createFilter(std::string name) {
    Filter *filter = NULL;
    if (name == "GaussianFilter") {
        filter = new GaussianFilter();
        //} else if (name == "GaussianFilter") {
    //    filter = GaussianFilter();

    //} else {
    //    filter = null;
    }
    return filter;

}



