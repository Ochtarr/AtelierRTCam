/*
 * DerivativeMask.cpp
 *
 *  Created on: 9 oct. 2017
 *      Author: antoine
 */

#include "DerivativeMask.h"
#include <iostream>
namespace cv {

DerivativeMask::~DerivativeMask() {
	// TODO Auto-generated destructor stub
}
void DerivativeMask::initGaussian(int rows, int cols, float sigma) {
	int mid_col = cols / 2;
	int mid_row = rows / 2;

	for (int i = -mid_row; i <= rows - mid_row; i++) {
		for (int j = -mid_col; j <= cols - mid_col; j++) {

			mat.at<float>(i + mid_row, j + mid_col) = -(i
					/ (2 * M_PI * pow(sigma, 2)))
					* expf(-(pow(i, 2) + pow(j, 2)) / (2 * sigma * sigma));

		}
	}
}
void DerivativeMask::convolve(cv::Mat& img, cv::Mat& img_res) {

	int kern_center_col = this->mat.cols / 2;
	int kern_center_row = this->mat.rows / 2;
	float kernel_sum = 0;

	for (int krow_cursor = 0; krow_cursor < (this->mat.rows);
				++krow_cursor) {

			for (int kcol_cursor = 0; kcol_cursor < (this->mat.cols);
					++kcol_cursor) {
				kernel_sum += this->mat.at<float>(krow_cursor , kcol_cursor);
			}
	}
	//factor de correction du kernel
	(kernel_sum == 0)?kernel_sum = 1:  kernel_sum = 1/kernel_sum;

//	std::cout << "src img type : "<< img.type() << std::endl;
//	std::cout << "dest img type : "<< img_res.type() << std::endl;
////	std::cout << this->mat.rows << std::endl;

	for (int row_cursor = 0; row_cursor < img.rows; ++row_cursor) {

		for (int col_cursor = 0; col_cursor < img.cols; ++col_cursor) {

			double tmp = 0;

			for (int krow_cursor = 0; krow_cursor < (this->mat.rows);
					++krow_cursor) {

				for (int kcol_cursor = 0; kcol_cursor < (this->mat.cols);
						++kcol_cursor) {

					// index of input signal, used for checking boundary
					int row_cursor_kern = (this->mat.rows) - 1 - krow_cursor;
					int col_cursor_kern = (this->mat.cols) - 1 - kcol_cursor;

					int row_cursor_convol = row_cursor
							+ (krow_cursor - kern_center_row);
					int col_cursor_convol = col_cursor
							+ (kcol_cursor - kern_center_col);
					// ignore input samples which are out of bound
					if (row_cursor_convol >= 0 && row_cursor_convol < (img.rows)
							&& col_cursor_convol >= 0
							&& col_cursor_convol < (img.cols)) {
						int val_img = (int) img.at<uchar>(row_cursor_convol,
								col_cursor_convol);
						int val_kernel =  this->mat.at<float>(
								row_cursor_kern, col_cursor_kern);

						tmp += val_img * val_kernel;
						std::cout << "img : "<< val_img <<  "kern : "<< val_kernel << "res : "<< tmp <<std::endl;

					}
				}
			}

			;
//			std::cout << tmp << std::endl;
			if (tmp < 0)
				tmp = 0;
			if (tmp > 255)
				tmp = 255;

			img_res.at<uchar>(row_cursor, col_cursor) = (uchar) tmp * kernel_sum;
//			 std::cout <<(int)img_res.at<uchar>(row_cursor , col_cursor ) << std::endl;

		}
	}

}

} /* namespace cv */
