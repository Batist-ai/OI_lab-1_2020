#include <math.h>
#include "pixel.h"
using namespace cv;

#define FILTERS_MF_DEFAULT_RADIUS 3
#define FILTERS_GF_DEFAULT_SIGMA 2.0

class Filter {

    protected: 
        virtual Pixel calculateNewPixel(Mat sourceImage, int x, int y) = 0;
        virtual Mat preprocessImage(Mat sourceImage);

    public: 
        Filter();

        int Clamp(int value, int min, int max);
        Mat processImage(Mat sourceImage);

};

class MatrixFilter : public Filter
{
    protected: 
        std::vector<float> m_kernel;
        size_t m_cols;
        size_t m_radius;
           
    public: 

        MatrixFilter();
        MatrixFilter(std::vector<float> kernel, size_t cols);
        MatrixFilter(size_t radius);

    protected:
        Pixel calculateNewPixel(Mat sourceImage, int x, int y);
        void print_kernel(std::string caption);

};

class GaussianFilter : public MatrixFilter {

    protected:
        float m_sigma;

    public:

        GaussianFilter();
        GaussianFilter(size_t radius, float sigma);

    private:
        void createGaussianKernel();
};

class MedianFilter: public MatrixFilter {

    private:
        void createKernel();

    public:
        MedianFilter();

    protected:
        Pixel calculateNewPixel(Mat sourceImage, int x, int y); 
};


class ProcessingTask {
    protected:
        std::vector<std::string> m_filters;
        Mat m_image;

    public:
        Mat get_image();
        void set_image(Mat image);
        std::vector<std::string> filters();
        void set_filters(std::vector<std::string> filters);
        ProcessingTask(Mat image, std::vector<std::string> filters);
        Filter *createFilter(std::string name);
};
