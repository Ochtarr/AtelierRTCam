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
	Serial arduino;
	arduino.Init();

	char *buff = new char[1];
	char *buffUD = new char[1];
	char *buffLR = new char[1];
	
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

		float seuilX = 50;
		float seuilY = 50;	

		if (ux != 0 || uy != 0)
		{
			if ((float)(img.rows/2)-ux > -seuilX && (float)(img.rows/2)-ux < seuilX)
				sprintf(buffLR, "%c", 0b11110000);
			else if ((float)(img.rows/2)-ux < 0)
				sprintf(buffLR, "%c", 0b11110111);
			else
			sprintf(buffLR, "%c", 0b11111111);

			if ((float)(img.cols/2)-uy > -seuilY && (float)(img.cols/2)-uy < seuilY)
				sprintf(buffUD, "%c", 0b00001111);
			else if ((float)(img.cols/2)-uy < 0)
				sprintf(buffUD, "%c", 0b11111111);
			else
				sprintf(buffUD, "%c", 0b01111111);
		}

		sprintf(buff, "%c", buffLR[0] & buffUD[0]);

		arduino.Write(buff);

		cv::imshow("w", img);

		key = cv::waitKey(20);
	}

	arduino.Close();
}
