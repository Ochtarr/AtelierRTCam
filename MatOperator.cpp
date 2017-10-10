/*
 * MatOperator.cpp
 *
 *  Created on: 10 oct. 2017
 *      Author: antoine
 */

#include "MatOperator.hpp"

namespace cv {

 void MatOperator::addMat( const cv::Mat& left , const cv::Mat& right ,  cv::Mat& res ){

	int size_col = (left.cols < right.cols)?left.cols : right.cols;
	int size_row = (left.rows < right.rows)?left.rows : right.rows;

	for(int row_cursor = 0 ;row_cursor < size_row ; ++row_cursor ){
		for(int col_cursor = 0 ;col_cursor < size_col ; ++col_cursor ){
				if(res.type() == CV_8U){

					int tmp = left.at<uchar>(row_cursor, col_cursor) + right.at<uchar>(row_cursor, col_cursor);
						if (tmp < 0)
								tmp = 0;
							if (tmp > 255)
								tmp = 255;

					res.at<uchar>(row_cursor, col_cursor) = (uchar)tmp;
				}
			}
	}


}
 void MatOperator::mulMat( const cv::Mat& left , const cv::Mat& right ,  cv::Mat& res ){

	int size_col = (left.cols < right.cols)?left.cols : right.cols;
	int size_row = (left.rows < right.rows)?left.rows : right.rows;

	for(int row_cursor = 0 ;row_cursor < size_row ; ++row_cursor ){
		for(int col_cursor = 0 ;col_cursor < size_col ; ++col_cursor ){
				if(res.type() == CV_8U){

					int tmp = left.at<uchar>(row_cursor, col_cursor) * right.at<uchar>(row_cursor, col_cursor);
						if (tmp < 0)
								tmp = 0;
							if (tmp > 255)
								tmp = 255;

					res.at<uchar>(row_cursor, col_cursor) = (uchar)tmp;
				}
			}
	}


}


} /* namespace cv */
