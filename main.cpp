#include <math.h>
#include <iostream>
#include <sys/time.h>
#include <opencv/cv.hpp>

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

	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
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

	   // On récupère une image
	   cap >> img;
	   cv::imshow("w", img);
	   waitKey(20);
	   // On affiche l'image dans une fenêtre

	   // On attend 10ms

	}
}
