#include <math.h>
#include <iostream>
#include <sys/time.h>
#include <opencv/cv.hpp>

#include <iostream>
#include "imgProcessing.hpp"
#include "Serial.hpp"
#include "PID.hpp"

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
	int key = 0;
	float ux = 0;
	float uy = 0;
	float ux_old = 0;
	float uy_old = 0;
	float ux_res = 0;
	float uy_res = 0;
	float threshold = 0.4f;

	cv::Mat img;

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

	VideoCapture cap(1); // open the default camera

	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

	if (!cap.isOpened())  // check if we succeeded
		return -1;

	cap >> img;
	Size new_size = img.size();
	int height = new_size.height;
	int width = new_size.width;

	ux_res = height;
	uy_res = width;
	ux = height;
	uy = width;
	ux_old = height;
	uy_old = width;
	//add antoine

	cv::Mat_<float> grad_vertical(height, width);
	cv::Mat_<float> grad_horizontal(height, width);
	cv::Mat_<float> grad_vertical_horizontal(height, width);
	cv::Mat_<float> grad_vertical_2(height, width);
	cv::Mat_<float> grad_horizontal_2(height, width);
	cv::Mat_<float> grad_vertical_horizontal_blurred(height, width);
	cv::Mat_<float> grad_vertical_2_blurred(height, width);
	cv::Mat_<float> grad_horizontal_2_blurred(height, width);

	cv::Mat img_res = cv::Mat::zeros(height, width, CV_8UC1);
	cv::Mat img_comp = cv::Mat::zeros(height, 2 * width, CV_8UC1);

	cv::Mat img_tab[2];
	cv::Mat img_grey_tab[2];
	cv::Mat_<float> img_harris_tab[2];
	img_harris_tab[0] = cv::Mat::zeros(height, width, CV_32S);
	img_harris_tab[1] = cv::Mat::zeros(height, width, CV_32S);
	int idx = 0;
	cap >> img_tab[idx];
	cvtColor(img_tab[idx], img_grey_tab[idx], cv::COLOR_RGB2GRAY);
	PID adapterCommand(1.2, 0.2, 0.05, height, width); //coeff P, largeur img, hauteur img
	int resultAdapter[4];

	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	while (key != 'q' && key != 'Q') {

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

		imgProcessing::GradientCornerDetection(&img_grey_tab[idx],
				&img_grey_tab[(idx + 1) % 2], &img_harris_tab[(idx + 1) % 2],
				&img_harris_tab[idx], ux_res, uy_res, threshold, 10, NULL,
				&img_comp, &img_res, &grad_vertical, &grad_horizontal,
				&grad_vertical_horizontal, &grad_vertical_2, &grad_horizontal_2,
				&grad_vertical_horizontal_blurred, &grad_vertical_2_blurred,
				&grad_horizontal_2_blurred

				);

		ux += ux_res;
		uy += uy_res;

		adapterCommand.Calcul(ux_old, uy_old, ux, uy, resultAdapter); //x1, y1, x2, y2

		if (resultAdapter[2] == 1)
			sprintf(buffLR, "%c", (resultAdapter[3] | 0b00000000) << 4);
		else
			sprintf(buffLR, "%c", (resultAdapter[3] | 0b00001000) << 4);

		if (resultAdapter[0] == 0)
			sprintf(buffUD, "%c", (resultAdapter[1] | 0b00000000));
		else
			sprintf(buffUD, "%c", (resultAdapter[1] | 0b00001000));

		sprintf(buff, "%c", buffLR[0] | buffUD[0]);
//	    printf("%d", buff[0]);

		imshow("comp", img_comp);

		imshow("harrisDetector", img_harris_tab[idx]);
		imshow("harrisDetectorprev", img_harris_tab[(idx + 1) % 2]);
		imshow("vector result", img_res);
		img_res = cv::Mat::zeros(height, width, CV_8UC1);
		key = cv::waitKey(20);
		arduino.Write(buff);
		ux_old = ux;
		uy_old = uy;
	}
	arduino.Close();
}
