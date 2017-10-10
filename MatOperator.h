/*
 * MatOperator.h
 *
 *  Created on: 10 oct. 2017
 *      Author: antoine
 */

#ifndef MATOPERATOR_H_
#define MATOPERATOR_H_
#include <opencv2/opencv.hpp>

namespace cv {

/*
 *
 */
class MatOperator {
public:
	static void addMat( const cv::Mat& left , const cv::Mat& right ,  cv::Mat& res );
};

} /* namespace cv */

#endif /* MATOPERATOR_H_ */
