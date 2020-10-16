#pragma once

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace qm
{
	// sigma on block_size
	double sigma(Mat & m, int i, int j, int block_size);

	// Covariance
	double cov(Mat & m1, Mat & m2, int i, int j, int block_size);

	// Mean squared error
	double eqm(Mat & img1, Mat & img2);

	/**
	 *	Compute the PSNR between 2 images
	 */
	double psnr(Mat & img_src, Mat & img_compressed, int block_size);

	/**
	 * Compute the SSIM between 2 images
	 */
	double ssim(Mat & img_src, Mat & img_compressed, int block_size, bool show_progress = false);

	void compute_quality_metrics(char * file1, char * file2, int block_size);
	void compute_quality_metrics(Mat img_src, Mat img_compressed, int block_size);
}
