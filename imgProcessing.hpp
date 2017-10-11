#ifndef IMGPROCESSING_HPP
#define IMGPROCESSING_HPP

#include <opencv/cv.hpp>
#include <vector>
namespace imgProcessing
{
	void redTracking(cv::Mat &picture, int lowThreshold, int highThreshold,float &ux,
			float &uy);


	void GradientCornerDetection(
			cv::Mat& img,
			cv::Mat_<float>* res_harris,
			cv::Mat_<float>* prev_res_harris,
			float &ux,
			float &uy,
			float threshold = 0.5,
			std::vector<cv::Point2d>* res = NULL,
			cv::Mat* img_res = NULL,
			cv::Mat_<float>* grad_vertical = NULL,
			cv::Mat_<float>* grad_horizontal = NULL,
			cv::Mat_<float>* grad_vertical_horizontal = NULL,
			cv::Mat_<float>* grad_vertical_2 = NULL,
			cv::Mat_<float>* grad_horizontal_2 = NULL,
			cv::Mat_<float>* grad_vertical_horizontal_blurred = NULL,
			cv::Mat_<float>* grad_vertical_2_blurred = NULL,
			cv::Mat_<float>* grad_horizontal_2_blurred = NULL );
}

#endif
