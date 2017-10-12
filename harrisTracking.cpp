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
	float threshold =  0.8f;

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






		   //add antoine


			cv::Mat_<float> grad_vertical(height , width );
			cv::Mat_<float> grad_horizontal(height , width);
			cv::Mat_<float> grad_vertical_horizontal(height , width);
			cv::Mat_<float> grad_vertical_2(height , width);
			cv::Mat_<float> grad_horizontal_2(height , width);
			cv::Mat_<float> grad_vertical_horizontal_blurred(height , width);
			cv::Mat_<float> grad_vertical_2_blurred(height , width);
			cv::Mat_<float> grad_horizontal_2_blurred(height , width);





			cv::Mat img_res(height , 2*width  , CV_8UC1);





cv::Mat img_tab[2];
cv::Mat img_grey_tab[2];
cv::Mat_<float> img_harris_tab[2];
img_harris_tab[0] = cv::Mat::zeros(height , width , CV_32S);
img_harris_tab[1] = cv::Mat::zeros(height , width , CV_32S);
int idx = 0;
cap >> img_tab[idx];
cvtColor(img_tab[idx], img_grey_tab[idx], cv::COLOR_RGB2GRAY);

	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	while(key != 'q' && key != 'Q')
	{

		++idx;
		idx = idx % 2;
		cap >> img_tab[idx];
//		std::cout<< idx <<endl;
		cvtColor(img_tab[idx], img_grey_tab[idx], cv::COLOR_RGB2GRAY);



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


		imgProcessing::GradientCornerDetection(
							&img_grey_tab[idx],
							&img_grey_tab[(idx+1)%2],
							&img_harris_tab[idx],
							&img_harris_tab[(idx+1)%2],
							 ux,
							 uy,
							 threshold,
							 10
							 ,NULL,
							&img_res,
							&grad_vertical,
							&grad_horizontal,
							&grad_vertical_horizontal,
							&grad_vertical_2,
							&grad_horizontal_2,
							&grad_vertical_horizontal_blurred,
							&grad_vertical_2_blurred,
							&grad_horizontal_2_blurred

							);

		 cout << "vx :" << ux << " vy :" <<uy << endl,

		imshow("original",img_res);

		imshow("harrisDetector",img_harris_tab[idx]);
		imshow("harrisDetectorprev",img_harris_tab[(idx+1)%2]);



	   key = cv::waitKey(20);


	}


}
