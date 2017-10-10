#include <math.h>
#include <iostream>
#include <sys/time.h>
#include <opencv/cv.hpp>
#include "DerivativeMask.h"
==== BASE ====
#include "MatOperator.h"
#include <iostream>
#include "MatOperator.hpp"



using namespace cv;
using namespace std;





int main(int argc, char **argv)
{
	int key = 0;
	cv::Mat img;
	

	#if FREQUENCY == 1
		struct timeval startTime;
		struct timeval currTime;
		int64_t start_usec;
		int64_t curr_usec;
		int frameNb = 0;
		int lap = 0;

		gettimeofday(&startTime, NULL);
		start_usec = pow(10,6)*startTime.tv_sec + startTime.tv_usec;		
	#endif

	VideoCapture cap(0); // open the default camera
	    if(!cap.isOpened())  // check if we succeeded
	        return -1;

	    namedWindow("w", 1);
	    namedWindow("wo", 1);
	    namedWindow("wa", 1);
	    namedWindow("wb", 1);
	    DerivativeMask kern(SOBEL_3_3_HORIZONTAL);
	    DerivativeMask kern2(SOBEL_3_3_VERTICAL);
	    DerivativeMask kern3(GAUSSIAN_3_3_SIGMA_3);

		   cap >> img;
		   Mat Temp(img.size(),CV_8U);
		   Mat convolution_vert(img.size(),CV_8U);
		   Mat convolution_horizontal(img.size(),CV_8U);
		   Mat convolution_sum(img.size(),CV_8U);
		   Mat convolution_blur(img.size(),CV_8U);

	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	while(key != 'q' && key != 'Q')
	{
		cap >> img;

		#if FREQUENCY == 1
			frameNb++;
			gettimeofday(&currTime, NULL);
			curr_usec = pow(10,6)*currTime.tv_sec + currTime.tv_usec;

			if(curr_usec-start_usec > pow(10,6))
			{
				
				lap++;
				startTime.tv_sec = currTime.tv_sec;
				startTime.tv_usec = currTime.tv_usec;
				start_usec = pow(10,6)*startTime.tv_sec + startTime.tv_usec;
				if(lap%10 == 0)
					printf("%d FPS\n", (int) ceil(1.*frameNb/lap));
			}		
		#endif

		cvtColor(img, Temp, cv::COLOR_RGB2GRAY);


//	   kern.convolve(Temp , convolution_vert);
//	   kern2.convolve(Temp , convolution_horizontal);
	   kern3.convolve(Temp , convolution_blur);
//	   MatOperator::addMat(convolution_vert , convolution_horizontal , convolution_sum);

//	   cv::imshow("w", convolution_vert);
//	   cv::imshow("wo", convolution_horizontal);
//	   cv::imshow("wa", convolution_sum);
	   cv::imshow("wb", convolution_blur);
//	   Sobel( Temp, convolution,  CV_16S, 1, 0, 3 );
	  // convertScaleAbs( convolution, convolution_res );

	   key = cv::waitKey(20);



	}
}
