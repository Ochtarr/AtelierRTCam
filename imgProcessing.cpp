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

	void GradientCornerDetection(
			cv::Mat& img,
			cv::Mat_<float>* res_harris,
			cv::Mat_<float>* prev_res_harris,
			float& ux,
			float& uy,
			float threshold ,

			std::vector<cv::Point2d>* res,
			cv::Mat* img_res,
			cv::Mat_<float>* grad_vertical,
			cv::Mat_<float>* grad_horizontal,
			cv::Mat_<float>* grad_vertical_horizontal,
			cv::Mat_<float>* grad_vertical_2,
			cv::Mat_<float>* grad_horizontal_2,
			cv::Mat_<float>* grad_vertical_horizontal_blurred,
			cv::Mat_<float>* grad_vertical_2_blurred,
			cv::Mat_<float>* grad_horizontal_2_blurred){

			bool grad_vertical_ini = false;
			bool grad_horizontal_ini = false;
			bool grad_vertical_horizontal_ini = false;
			bool grad_vertical_2_ini = false;
			bool grad_horizontal_2_ini = false;
			bool grad_vertical_horizontal_blurred_ini = false;
			bool grad_vertical_2_blurred_ini = false;
			bool grad_horizontal_2_blurred_ini = false;


			if (grad_vertical == NULL) {
				grad_vertical = new cv::Mat_<float>(img.rows, img.cols);
				grad_vertical_ini = true;
			}
			if (grad_horizontal == NULL) {
				grad_horizontal = new cv::Mat_<float>(img.rows, img.cols);
				grad_horizontal_ini = true;
			}
			if (grad_vertical_horizontal == NULL) {
				grad_vertical_horizontal = new cv::Mat_<float>(img.rows, img.cols);
				grad_vertical_horizontal_ini = true;
			}
			if (grad_vertical_2 == NULL) {
				grad_vertical_2 = new cv::Mat_<float>(img.rows, img.cols);
				grad_vertical_2_ini = true;
			}
			if (grad_horizontal_2 == NULL) {
				grad_horizontal_2 = new cv::Mat_<float>(img.rows, img.cols);
				grad_horizontal_2_ini = true;

			}

			if (grad_vertical_horizontal_blurred == NULL) {
				grad_vertical_horizontal_blurred = new cv::Mat_<float>(img.rows,
						img.cols);
				grad_vertical_horizontal_blurred_ini = true;
			}
			if (grad_vertical_2_blurred == NULL) {
				grad_vertical_2_blurred = new cv::Mat_<float>(img.rows, img.cols);
				grad_vertical_2_blurred_ini = true;
			}
			if (grad_horizontal_2_blurred == NULL) {
				grad_horizontal_2_blurred = new cv::Mat_<float>(img.rows, img.cols);
				grad_horizontal_2_blurred_ini = true;
			}
			if(threshold  == NULL  )
				threshold = 0;



			int kern_center_col = 1;
			int kern_center_row = 1;
			float kernel_sum = 8;

		#ifdef DEBUG
			std::cout << "src img type : "<< img.type() << std::endl;
		#endif

			//compute all the derivative

			for (int row_cursor = kern_center_row;
					row_cursor < img.rows - kern_center_row; ++row_cursor) {

				for (int col_cursor = kern_center_col;
						col_cursor < img.cols - kern_center_col; ++col_cursor) {

					//compute the row derivative
					float row_grad = img.at<uchar>(row_cursor - 1, col_cursor - 1)
							* -1.0 + img.at<uchar>(row_cursor, col_cursor - 1) * -2.0
							+ img.at<uchar>(row_cursor + 1, col_cursor - 1) * -1.0
							+
		//					img.at<uchar>(row_cursor-1, 	col_cursor) 	* 0 +
		//					img.at<uchar>(row_cursor, 		col_cursor) 	* 0 +  // the midle case is null
		//					img.at<uchar>(row_cursor +1, 	col_cursor) 	* 0 +
							img.at<uchar>(row_cursor - 1, col_cursor + 1) * 1.0
							+ img.at<uchar>(row_cursor, col_cursor + 1) * 2.0
							+ img.at<uchar>(row_cursor + 1, col_cursor + 1) * 1.0;

					//compute the col derivative
					float col_grad = img.at<uchar>(row_cursor - 1, col_cursor - 1) * -1

		//					+ img.at<uchar>(row_cursor, 		col_cursor-1) 	* 0
							+ img.at<uchar>(row_cursor + 1, col_cursor - 1) * 1
							+ img.at<uchar>(row_cursor - 1, col_cursor) * -2
		//					+ img.at<uchar>(row_cursor, col_cursor) 	* 0
							+ img.at<uchar>(row_cursor + 1, col_cursor) * 2
							+ img.at<uchar>(row_cursor - 1, col_cursor + 1) * -1
		//					+ img.at<uchar>(row_cursor, 		col_cursor+1) 	* 0
							+ img.at<uchar>(row_cursor + 1, col_cursor + 1) * 1;

					col_grad = col_grad / 255;
					row_grad = row_grad / 255;

					float row_grad_2 = row_grad * row_grad;
					float col_grad_2 = col_grad * col_grad;
					float row_col_grad = col_grad * row_grad;
					grad_vertical->at<float>(row_cursor, col_cursor) = row_grad;
					grad_vertical_2->at<float>(row_cursor, col_cursor) = row_grad_2;
					grad_horizontal->at<float>(row_cursor, col_cursor) = col_grad;
					grad_horizontal_2->at<float>(row_cursor, col_cursor) = col_grad_2;
					grad_vertical_horizontal->at<float>(row_cursor, col_cursor) =
							row_col_grad;

				}
			}

			for (int row_cursor = kern_center_row;
					row_cursor < img.rows - kern_center_row; ++row_cursor) {

				for (int col_cursor = kern_center_col;
						col_cursor < img.cols - kern_center_col; ++col_cursor) {

					float row_grad_2_blur = grad_vertical_2->at<float>(row_cursor - 1,
							col_cursor - 1) * 1
							+ grad_vertical_2->at<float>(row_cursor, col_cursor - 1)
							+ grad_vertical_2->at<float>(row_cursor + 1, col_cursor - 1)

							+ grad_vertical_2->at<float>(row_cursor - 1, col_cursor)
							+
		//							grad_vertical_2->at<float>(row_cursor, 		col_cursor) 	* 0 +  // the midle case is null
							grad_vertical_2->at<float>(row_cursor + 1, col_cursor)
							+ grad_vertical_2->at<float>(row_cursor - 1, col_cursor + 1)

							+ grad_vertical_2->at<float>(row_cursor, col_cursor + 1)
							+ grad_vertical_2->at<float>(row_cursor + 1, col_cursor + 1)
									* 1;
					float col_grad_2_blur = grad_horizontal_2->at<float>(row_cursor - 1,
							col_cursor - 1)
							+ grad_horizontal_2->at<float>(row_cursor, col_cursor - 1)

							+ grad_horizontal_2->at<float>(row_cursor + 1,
									col_cursor - 1)
							+ grad_horizontal_2->at<float>(row_cursor - 1, col_cursor)

							+
		//							grad_horizontal_2->at<float>(row_cursor, 		col_cursor) 	* 0 +  // the midle case is null
							grad_horizontal_2->at<float>(row_cursor + 1, col_cursor) * 1
							+ grad_horizontal_2->at<float>(row_cursor - 1,
									col_cursor + 1)
							+ grad_horizontal_2->at<float>(row_cursor, col_cursor + 1)
									* 1
							+ grad_horizontal_2->at<float>(row_cursor + 1,
									col_cursor + 1);
					float col_row_grad_blur = grad_vertical_horizontal->at<float>(
							row_cursor - 1, col_cursor - 1) * 1
							+ grad_vertical_horizontal->at<float>(row_cursor,
									col_cursor - 1)
							+ grad_vertical_horizontal->at<float>(row_cursor + 1,
									col_cursor - 1)
							+ grad_vertical_horizontal->at<float>(row_cursor - 1,
									col_cursor)
							+
		//							grad_vertical_horizontal->at<float>(row_cursor, 		col_cursor) 	* 0 +  // the midle case is null
							grad_vertical_horizontal->at<float>(row_cursor + 1,
									col_cursor)
							+ grad_vertical_horizontal->at<float>(row_cursor - 1,
									col_cursor + 1)
							+ grad_vertical_horizontal->at<float>(row_cursor,
									col_cursor + 1)
							+ grad_vertical_horizontal->at<float>(row_cursor + 1,
									col_cursor + 1);

					col_grad_2_blur = col_grad_2_blur / kernel_sum;
					row_grad_2_blur = row_grad_2_blur / kernel_sum;
					col_row_grad_blur = col_row_grad_blur / kernel_sum;

					grad_horizontal_2_blurred->at<float>(row_cursor, col_cursor) =
							col_grad_2_blur;
					grad_vertical_2_blurred->at<float>(row_cursor, col_cursor) =
							row_grad_2_blur;
					grad_vertical_horizontal_blurred->at<float>(row_cursor, col_cursor) =
							col_row_grad_blur;

					//Ix^2 <Iy^2>  + Iy^2 <Ix^2> - 2IxIy <IxIy>
					//-------------------------------------------
					//				<Ix^2> <Iy^2>

					float k = (
					//Ix^2 <Iy^2>
							grad_vertical_2->at<float>(row_cursor, col_cursor)
									* col_grad_2_blur
							//+ Iy^2 <Ix^2>
									+ grad_horizontal_2->at<float>(row_cursor,
											col_cursor) * row_grad_2_blur
							//- 2IxIy <IxIy>
									- 2
											* grad_vertical_horizontal->at<float>(
													row_cursor, col_cursor)
											* col_row_grad_blur);
		//					/ (
		//			//		<Ix^2> <Iy^2>
		//					col_grad_2_blur * row_grad_2_blur);
					k = fabs(k);
					if (k < threshold) {
						k = 0.0;
					} else {
						if (res != NULL) {
							res->push_back(cv::Point2d(row_cursor, col_cursor));
						}
						if (img_res != NULL) {
#if GRAPHIC ==  1
							cv::circle(*img_res, cv::Point(col_cursor, row_cursor),1, cv::Scalar(0,0,255));
#endif

						}
					}

						res_harris->at<float>(row_cursor, col_cursor) = k;



				}
			}

			if (grad_vertical_ini) {
				delete grad_vertical;
			}
			if (grad_horizontal_ini) {
				delete grad_horizontal;
			}
			if (grad_vertical_horizontal_ini) {
				delete grad_vertical_horizontal;
			}
			if (grad_vertical_2_ini) {
				delete grad_vertical_2;
			}
			if (grad_horizontal_2_ini) {
				delete grad_horizontal_2;
			}
			if (grad_vertical_horizontal_blurred_ini) {
				delete grad_vertical_horizontal_blurred;
			}
			if (grad_vertical_2_blurred_ini) {
				delete grad_vertical_2_blurred;
			}
			if (grad_horizontal_2_blurred_ini) {
				delete grad_horizontal_2_blurred;
			}

	}
}
