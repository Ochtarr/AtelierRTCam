#include "imgProcessing.hpp"
#include <iostream>
#include <math.h>

namespace imgProcessing
{
	void redTracking(cv::Mat &picture, int lowThreshold, int highThreshold, float &ux, float &uy)
	{
		int pixelNb = 0;
		uchar r;
		uchar g;
		uchar b;

		ux = 0;
		uy = 0;

		for(int i = 0; i < picture.rows; i++)
		{
			for(int j =0;	j < picture.cols; j++)
			{
				b = picture.at<cv::Vec3b>(i,j)[0]; 
				g = picture.at<cv::Vec3b>(i,j)[1];
				r = picture.at<cv::Vec3b>(i,j)[2];

				if(b < lowThreshold && g < lowThreshold && r > highThreshold)
				{
					ux += j;
					uy += i;
					pixelNb++;
				}
				else
				{
					int greyLvl = floor(1.*(r+g+b)/3);

					picture.at<cv::Vec3b>(i,j)[0] = greyLvl;
					picture.at<cv::Vec3b>(i,j)[1] = greyLvl;
					picture.at<cv::Vec3b>(i,j)[2] = greyLvl;
				}
			}
		}

		ux /= pixelNb;
		uy /= pixelNb;
	}
}
