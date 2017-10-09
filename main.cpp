#include <stdlib.h>
#include <stdio.h>
#include <opencv/cv.hpp>





using namespace cv;





int main(int argc, char **argv)
{

	int key = 0;
	cv::Mat img;

	VideoCapture cap(0); // open the default camera
	    if(!cap.isOpened())  // check if we succeeded
	        return -1;

	    namedWindow("w", 1);
	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	while(key != 'q' && key != 'Q') {

	   // On récupère une image
	   cap >> img;
	   cv::imshow("w", img);
	     waitKey(20);
	   // On affiche l'image dans une fenêtre

	   // On attend 10ms

	}
}

