#ifndef IMGPROCESSING_HPP
#define IMGPROCESSING_HPP

#include <opencv/cv.hpp>

namespace imgProcessing
{
	void redTracking(cv::Mat &picture, int lowThreshold, int highThreshold, float &ux, float &uy);
}

#endif
