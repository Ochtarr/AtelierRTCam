/*
 * DerivativeMask.h
 *
 *  Created on: 9 oct. 2017
 *      Author: antoine
 */

#ifndef DERIVATIVEMASK_H_
#define DERIVATIVEMASK_H_
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include <math.h>
namespace cv {

/*
 *
 */

enum MaskType{
	SOBEL_3_3_HORIZONTAL,
	SOBEL_3_3_VERTICAL,
	GAUSSIAN_3_3_SIGMA_3
};

class DerivativeMask {
private:
	MaskType _maskType;
	Mat mat;
	void initGaussian(int rows , int cols , float sigma);
public:



	DerivativeMask(MaskType  maskType ): _maskType(maskType) {


		switch(maskType){

		case SOBEL_3_3_HORIZONTAL:
			this->mat =  (Mat_<float>(3,3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
			break;
		case SOBEL_3_3_VERTICAL:
			this->mat =  (Mat_<float>(3,3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);;
			break;
		case GAUSSIAN_3_3_SIGMA_3:
			this->mat =  Mat_<float>(3,3);
			initGaussian(3,3,3);



			break;
		}



	}
	virtual ~DerivativeMask();
	cv::Mat getMask(){
		return this->mat;
	}



	void  convolve(Mat& img, Mat& img_res);

};




} /* namespace cv */

#endif /* DERIVATIVEMASK_H_ */
