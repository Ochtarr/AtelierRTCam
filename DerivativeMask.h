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
namespace cv {

/*
 *
 */

enum MaskType{
	SOBEL_3_3_HORIZONTAL,
	SOBEL_3_3_VERTICAL
};

class DerivativeMask {
private:
	MaskType _maskType;
	Mat mat;
public:



	DerivativeMask(MaskType  maskType ): _maskType(maskType) {


		switch(maskType){

		case SOBEL_3_3_HORIZONTAL:
			this->mat =  (Mat_<int>(3,3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
			break;
		case SOBEL_3_3_VERTICAL:
			this->mat =  (Mat_<int>(3,3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);;
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
