#include <math.h>
#include <iostream>
#include <sys/time.h>
#include <opencv/cv.hpp>

#include <iostream>
#include "imgProcessing.hpp"



using namespace cv;
using namespace std;







int main(int argc, char **argv)
{
	int key = 0;
	float ux = 0;
	float uy = 0;
	float threshold =  0.5f;

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

	cap.set(CV_CAP_PROP_FRAME_WIDTH,320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,240);

	    if(!cap.isOpened())  // check if we succeeded
	        return -1;




		   cap >> img;
		   Size new_size =  img.size();
		   int height = new_size.height;
		   int width = new_size.width;
		   Mat Temp(new_size,CV_8U);
		   Mat img_color(img.size(),CV_8UC3 );


		   vector<cv::Point2d> res;



		   //add antoine
			cv::Mat_<float>  res_harris( height , width);
			cv::Mat_<float> prev_res_harris( height , width);
;
			cv::Mat_<float> grad_vertical(height , width );
			cv::Mat_<float> grad_horizontal(height , width);
			cv::Mat_<float> grad_vertical_horizontal(height , width);
			cv::Mat_<float> grad_vertical_2(height , width);
			cv::Mat_<float> grad_horizontal_2(height , width);
			cv::Mat_<float> grad_vertical_horizontal_blurred(height , width);
			cv::Mat_<float> grad_vertical_2_blurred(height , width);
			cv::Mat_<float> grad_horizontal_2_blurred(height , width);













	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	while(key != 'q' && key != 'Q')
	{
		cap >> img;
		img.copyTo(img_color);


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
cv::Mat tmp2(height , width  , CV_32F);
		cvtColor(img, Temp, cv::COLOR_RGB2GRAY);
		imgProcessing::GradientCornerDetection(
							Temp,
							 &res_harris,
							 &prev_res_harris,
							 ux,
							 uy,
							 threshold
							 ,NULL,
							&img_color
//							&grad_vertical,
//							&grad_horizontal,
//							&grad_vertical_horizontal,
//							&grad_vertical_2,
//							&grad_horizontal_2,
//							&grad_vertical_horizontal_blurred,
//							&grad_vertical_2_blurred,
//							&grad_horizontal_2_blurred

							);



		imshow("original",img);

		imshow("harrisDetector",res_harris);
		imshow("harris img",img_color);


	   key = cv::waitKey(20);



	}


}
