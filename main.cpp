#include <math.h>
#include <iostream>
#include <sys/time.h>
#include <opencv/cv.hpp>

#include "MatOperator.hpp"
#include "DerivativeMask.h"
#include "imgProcessing.hpp"
#include "Serial.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	char *buff = new char[1];
	Serial arduino;
	
	if (arduino.Init() == 1) printf("Initialisation COM OK :)\n");
	else printf("Initialisation COM KO :[\n");
	
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

	float ux = 0, uy = 0;
	VideoCapture cap(0);
	
	cv::Mat img;
	int key = 0;

	if(!cap.isOpened())  // check if we succeeded
	{
		return -1;
	}

	namedWindow("w", 1);

	while(key != 'q' && key != 'Q')
	{
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

		cap >> img;

		imgProcessing::redTracking(img, 100, 120, ux, uy);

		float seuilX = 30;
		float seuilY = 30;

		//sprintf(buff, "%c", 0b10110001);
		//arduino.Write(buff);

		if ((float)(img.rows/2)-ux > -seuilX && (float)(img.rows/2)-ux < seuilX)
		{
			printf("NE RIEN FAIRE LR\n");
		}
		else if ((float)(img.rows/2)-ux < 0)
		{
			printf("GAUCHE LR\n");
		}
		else
		{
			printf("DROITE LR\n");
		}

		if ((float)(img.cols/2)-uy > -seuilY && (float)(img.cols/2)-uy < seuilY)
		{
			printf("NE RIEN FAIRE UP\n\n");
		}
		else if ((float)(img.cols/2)-uy < 0)
		{
			printf("HAUT UP\n");
		}
		else
		{
			printf("BAS UP\n");
		}

	   cv::imshow("w", img);

	   key = cv::waitKey(20);
	}

	arduino.Close();
}
