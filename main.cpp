#include <stdlib.h>
#include <stdio.h>
#include <opencv/cv.hpp>
#include "DerivativeMask.h"
#include "MatOperator.h"
#include <iostream>



using namespace cv;
using namespace std;





int main(int argc, char **argv)
{

	int key = 0;
	cv::Mat img;

	VideoCapture cap(0); // open the default camera
	    if(!cap.isOpened())  // check if we succeeded
	        return -1;

	    namedWindow("w", 1);
	    namedWindow("wo", 1);
	    namedWindow("wa", 1);
	    DerivativeMask kern(SOBEL_3_3_HORIZONTAL);
	    DerivativeMask kern2(SOBEL_3_3_VERTICAL);

		   cap >> img;
		   Mat Temp(img.size(),CV_8U);
		   Mat convolution_vert(img.size(),CV_8U);
		   Mat convolution_horizontal(img.size(),CV_8U);
		   Mat convolution_sum(img.size(),CV_8U);

	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	while(key != 'q' && key != 'Q') {
		   cap >> img;
	   // On récupère une image

	   cvtColor(img, Temp, cv::COLOR_RGB2GRAY);


	   kern.convolve(Temp , convolution_vert);
	   kern2.convolve(Temp , convolution_horizontal);
	   MatOperator::addMat(convolution_vert , convolution_horizontal , convolution_sum);

	   cv::imshow("w", convolution_vert);
	   cv::imshow("wo", convolution_horizontal);
	   cv::imshow("wa", convolution_sum);
//	   Sobel( Temp, convolution,  CV_16S, 1, 0, 3 );
	  // convertScaleAbs( convolution, convolution_res );

	   key = cv::waitKey(20);



	}
}

