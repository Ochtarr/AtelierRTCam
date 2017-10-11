/*
 * Harris.h
 *
 *  Created on: 10 oct. 2017
 *      Author: antoine
 */

#ifndef CORNERDETECTOR_H_
#define CORNERDETECTOR_H_
#include <opencv2/opencv.hpp>
#include <vector>

namespace std {

/*
 *
 */
class CornerDetector {
public:
static void GradientCornerDetection(cv::Mat& img ,
		cv::Mat_<float>* res_harris,
		vector<cv::Point2d>* res = NULL,
		cv::Mat* img_color = NULL,
		cv::Mat_<float>* grad_vertical  = NULL,
		cv::Mat_<float>* grad_horizontal = NULL,
		cv::Mat_<float>* grad_vertical_horizontal = NULL,
		cv::Mat_<float>* grad_vertical_2  = NULL,
		cv::Mat_<float>* grad_horizontal_2 = NULL,
		cv::Mat_<float>* grad_vertical_horizontal_2  = NULL,
		cv::Mat_<float>* grad_vertical_blurred  = NULL,
		cv::Mat_<float>* grad_horizontal_blurred = NULL ,
		cv::Mat_<float>* grad_vertical_horizontal_blurred = NULL,
		cv::Mat_<float>* grad_vertical_2_blurred  = NULL,
		cv::Mat_<float>* grad_horizontal_2_blurred = NULL,
		cv::Mat_<float>* grad_vertical_horizontal_2_blurred = NULL

		);
static void HarrisCornerDetection(cv::Mat& img ,
		float lambda = 1.0,
		cv::Mat_<float>* res_harris = NULL,
		cv::Mat* img_color = NULL,
		cv::Mat_<float>* grad_vertical = NULL,
		cv::Mat_<float>* grad_horizontal = NULL,
		cv::Mat_<float>* grad_vertical_horizontal = NULL,
		cv::Mat_<float>* grad_vertical_2 = NULL,
		cv::Mat_<float>* grad_horizontal_2 = NULL,
		cv::Mat_<float>* grad_vertical_horizontal_2 = NULL,
		cv::Mat_<float>* grad_vertical_blurred = NULL,
		cv::Mat_<float>* grad_horizontal_blurred = NULL,
		cv::Mat_<float>* grad_vertical_horizontal_blurred = NULL,
		cv::Mat_<float>* grad_vertical_2_blurred = NULL,
		cv::Mat_<float>* grad_horizontal_2_blurred = NULL,
		cv::Mat_<float>* grad_vertical_horizontal_2_blurred = NULL
		);




};

} /* namespace cv */

#endif /* CORNERDETECTOR_H_ */
