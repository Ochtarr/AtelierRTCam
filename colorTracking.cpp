/**
 * \file colorTracking.cpp
 * \brief Main colorTracking
 * \author All teams
 * \version 0.2
 * \date 11 octobre 2017
 *
 * Tracking de couleur rouge. Centre de gravité comparé au centre de l'image + adaptation des commandes des servos.
 *
 */

#include <math.h>
#include <iostream>
#include <sys/time.h>
#include <opencv/cv.hpp>
#include <chrono>
#include <thread>


#include "MatOperator.hpp"

#include "imgProcessing.hpp"
#include "Serial.hpp"
#include "PID.hpp"

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
	VideoCapture cap(1);
	
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

		std::cout <<  ux - img.cols/2 << " " <<  uy - img.rows/2<< endl;
		if (1 == 1)
		//if (ux > 0 && uy > 0)
		{
			PID adapterCommand(1.2, 0.2, 0.05, img.cols, img.rows); //coeff P, largeur img, hauteur img
			int resultAdapter[4];

			adapterCommand.Calcul(img.cols/2, img.rows/2, ux, uy, resultAdapter); //x1, y1, x2, y2
			
			if (resultAdapter[2] == 1) sprintf(buffLR, "%c", (resultAdapter[3] | 0b00000000)<<4);
			else sprintf(buffLR, "%c", (resultAdapter[3] | 0b00001000)<<4);
			
			if (resultAdapter[0] == 0) sprintf(buffUD, "%c", (resultAdapter[1] | 0b00000000));
			else sprintf(buffUD, "%c", (resultAdapter[1] | 0b00001000));

			sprintf(buff, "%c", buffLR[0] | buffUD[0]);

			arduino.Write(buff);

			cv::imshow("w", img);
//				std::this_thread::sleep_for(std::chrono::milliseconds(2000));



			key = cv::waitKey(20);
		}
	}

	arduino.Close();
}
