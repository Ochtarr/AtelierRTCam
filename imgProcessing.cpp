#include "imgProcessing.hpp"
#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>

namespace imgProcessing {
void redTracking(cv::Mat &picture, int lowThreshold, int highThreshold,
		float &ux, float &uy) {
	int pixelNb = 0;
	uchar r;
	uchar g;
	uchar b;

	ux = 0;
	uy = 0;

	for (int i = 0; i < picture.rows; i++) {
		for (int j = 0; j < picture.cols; j++) {
			b = picture.at<cv::Vec3b>(i, j)[0];
			g = picture.at<cv::Vec3b>(i, j)[1];
			r = picture.at<cv::Vec3b>(i, j)[2];

			if (b < lowThreshold && g < lowThreshold && r > highThreshold) {
				ux += j;
				uy += i;
				pixelNb++;
			} else {
				int greyLvl = floor(1. * (r + g + b) / 3);

				picture.at<cv::Vec3b>(i, j)[0] = greyLvl;
				picture.at<cv::Vec3b>(i, j)[1] = greyLvl;
				picture.at<cv::Vec3b>(i, j)[2] = greyLvl;
			}
		}
	}

	ux /= pixelNb;
	uy /= pixelNb;
}

void GradientCornerDetection(cv::Mat* img, cv::Mat* prev_img,
		cv::Mat_<float>* res_harris, cv::Mat_<float>* prev_res_harris,
		float& ux, float& uy, float threshold, float windows_size,
		std::vector<cv::Point2d>* res, cv::Mat* img_res,
		cv::Mat_<float>* grad_vertical, cv::Mat_<float>* grad_horizontal,
		cv::Mat_<float>* grad_vertical_horizontal,
		cv::Mat_<float>* grad_vertical_2, cv::Mat_<float>* grad_horizontal_2,
		cv::Mat_<float>* grad_vertical_horizontal_blurred,
		cv::Mat_<float>* grad_vertical_2_blurred,
		cv::Mat_<float>* grad_horizontal_2_blurred) {

	bool grad_vertical_ini = false;
	bool grad_horizontal_ini = false;
	bool grad_vertical_horizontal_ini = false;
	bool grad_vertical_2_ini = false;
	bool grad_horizontal_2_ini = false;
	bool grad_vertical_horizontal_blurred_ini = false;
	bool grad_vertical_2_blurred_ini = false;
	bool grad_horizontal_2_blurred_ini = false;

	int rows = img->rows;
	int cols = img->cols;

	cv::Mat speed_vector_table = cv::Mat::zeros(rows * 2, cols * 2, CV_32SC1);

	if (grad_vertical == NULL) {
		grad_vertical = new cv::Mat_<float>(rows, cols);
		grad_vertical_ini = true;
	}
	if (grad_horizontal == NULL) {
		grad_horizontal = new cv::Mat_<float>(rows, cols);
		grad_horizontal_ini = true;
	}
	if (grad_vertical_horizontal == NULL) {
		grad_vertical_horizontal = new cv::Mat_<float>(rows, cols);
		grad_vertical_horizontal_ini = true;
	}
	if (grad_vertical_2 == NULL) {
		grad_vertical_2 = new cv::Mat_<float>(rows, cols);
		grad_vertical_2_ini = true;
	}
	if (grad_horizontal_2 == NULL) {
		grad_horizontal_2 = new cv::Mat_<float>(rows, cols);
		grad_horizontal_2_ini = true;

	}

	if (grad_vertical_horizontal_blurred == NULL) {
		grad_vertical_horizontal_blurred = new cv::Mat_<float>(rows, cols);
		grad_vertical_horizontal_blurred_ini = true;
	}
	if (grad_vertical_2_blurred == NULL) {
		grad_vertical_2_blurred = new cv::Mat_<float>(rows, cols);
		grad_vertical_2_blurred_ini = true;
	}
	if (grad_horizontal_2_blurred == NULL) {
		grad_horizontal_2_blurred = new cv::Mat_<float>(rows, cols);
		grad_horizontal_2_blurred_ini = true;
	}
	if (threshold == NULL)
		threshold = 0;

	int kern_center_col = 1;
	int kern_center_row = 1;
	int speed_vector_max = 0;

	int speed_vector_max_row = 0;
	int speed_vector_max_col = 0;
	int poi_winner_row = -1;
	int poi_winner_col= -1;
	int poi_new_winner_row = -1 ;
	int poi_new_winner_col = -1;
	float kernel_sum = 8;

#ifdef DEBUG
	std::cout << "src img type : "<< img->type() << std::endl;
#endif

	//compute all the derivative

	for (int row_cursor = kern_center_row; row_cursor < rows - kern_center_row;
			++row_cursor) {

		for (int col_cursor = kern_center_col;
				col_cursor < cols - kern_center_col; ++col_cursor) {
			//reset the speed table

			speed_vector_table.at<int>(row_cursor, col_cursor) = 0;
			speed_vector_table.at<int>(row_cursor, 2 * col_cursor) = 0;
			speed_vector_table.at<int>(2 * row_cursor, col_cursor) = 0;
			speed_vector_table.at<int>(2 * row_cursor, 2 * col_cursor) = 0;
			//compute the row derivative
			float row_grad = img->at<uchar>(row_cursor - 1, col_cursor - 1)
					* -1.0 + img->at<uchar>(row_cursor, col_cursor - 1) * -2.0
					+ img->at<uchar>(row_cursor + 1, col_cursor - 1) * -1.0
					+
					//					img->at<uchar>(row_cursor-1, 	col_cursor) 	* 0 +
					//					img->at<uchar>(row_cursor, 		col_cursor) 	* 0 +  // the midle case is null
					//					img->at<uchar>(row_cursor +1, 	col_cursor) 	* 0 +
					img->at<uchar>(row_cursor - 1, col_cursor + 1) * 1.0
					+ img->at<uchar>(row_cursor, col_cursor + 1) * 2.0
					+ img->at<uchar>(row_cursor + 1, col_cursor + 1) * 1.0;

			//compute the col derivative
			float col_grad = img->at<uchar>(row_cursor - 1, col_cursor - 1) * -1

					//					+ img->at<uchar>(row_cursor, 		col_cursor-1) 	* 0
					+ img->at<uchar>(row_cursor + 1, col_cursor - 1) * 1
					+ img->at<uchar>(row_cursor - 1, col_cursor) * -2
					//					+ img->at<uchar>(row_cursor, col_cursor) 	* 0
					+ img->at<uchar>(row_cursor + 1, col_cursor) * 2
					+ img->at<uchar>(row_cursor - 1, col_cursor + 1) * -1
					//					+ img->at<uchar>(row_cursor, 		col_cursor+1) 	* 0
					+ img->at<uchar>(row_cursor + 1, col_cursor + 1) * 1;

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
#if GRAPHIC ==  1
			img_res->at<uchar>(row_cursor, col_cursor)=  prev_img->at<uchar>(row_cursor, col_cursor);
			img_res->at<uchar>(row_cursor,  cols + col_cursor) =  img->at<uchar>(row_cursor, col_cursor);
#endif


		}
	}

	int pt_interet = 0;
	for (int row_cursor = kern_center_row; row_cursor < rows - kern_center_row;
			++row_cursor) {

		for (int col_cursor = kern_center_col;
				col_cursor < cols - kern_center_col; ++col_cursor) {

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

			int min_difference = INT32_MAX;
			int min_similarity_row = -1;
			int min_similarity_col = -1;
			k = fabs(k);
			if (k < threshold) {
				k = 0.0;
			} else {
				pt_interet++;
				if (res != NULL) {
					res->push_back(cv::Point2d(row_cursor, col_cursor));
				}
				if (img_res != NULL) {
#if GRAPHIC ==  1
					cv::circle(*img_res, cv::Point(col_cursor, row_cursor),1, cv::Scalar(0,0,255));
#endif

				}

				int lower_row_prev_cur =
						(0 < row_cursor - windows_size) ?
								row_cursor - windows_size : 0;
				int upper_row_prev_cur =
						(rows > row_cursor + windows_size) ?
								row_cursor + windows_size : rows;
				int lower_col_prev_cur =
						(0 < col_cursor - windows_size) ?
								col_cursor - windows_size : 0;
				int upper_col_prev_cur =
						(cols > col_cursor - windows_size) ?
								col_cursor + windows_size : cols;

				float difference;
				int counter_point;
				if (prev_img != NULL) {

					//on cherche le point d'interet le plus imilaire dans l'entourage de la fonction
					for (int row_prev_cursor = lower_row_prev_cur;
							row_prev_cursor < upper_row_prev_cur;
							row_prev_cursor++) {
						for (int col_prev_cursor = lower_col_prev_cur;
								col_prev_cursor < upper_col_prev_cur;
								col_prev_cursor++) {

							if (prev_res_harris->at<float>(row_prev_cursor,
									col_prev_cursor) != 0.0) {
//
								counter_point++;

								difference =
										sqrt(
												pow(
														prev_img->at<float>(
																row_prev_cursor
																		- 1,
																col_prev_cursor
																		- 1)
																- img->at<float>(
																		row_prev_cursor
																				- 1,
																		col_prev_cursor
																				- 1),
														2))
												+ sqrt(
														pow(
																prev_img->at<
																		float>(
																		row_prev_cursor,
																		col_prev_cursor
																				- 1)
																		- img->at<
																				float>(
																				row_prev_cursor,
																				col_prev_cursor
																						- 1),
																2))
												+ sqrt(
														pow(
																prev_img->at<
																		float>(
																		row_prev_cursor
																				+ 1,
																		col_prev_cursor
																				- 1)
																		- img->at<
																				float>(
																				row_prev_cursor
																						+ 1,
																				col_prev_cursor
																						- 1),
																2))
												+ sqrt(
														pow(
																prev_img->at<
																		float>(
																		row_prev_cursor
																				- 1,
																		col_prev_cursor)
																		- img->at<
																				float>(
																				row_prev_cursor
																						- 1,
																				col_prev_cursor),
																2))
												+ sqrt(
														pow(
																prev_img->at<
																		float>(
																		row_prev_cursor,
																		col_prev_cursor)
																		- img->at<
																				float>(
																				row_prev_cursor,
																				col_prev_cursor),
																2))
												+ sqrt(
														pow(
																prev_img->at<
																		float>(
																		row_prev_cursor
																				+ 1,
																		col_prev_cursor)
																		- img->at<
																				float>(
																				row_prev_cursor
																						+ 1,
																				col_prev_cursor),
																2))
												+ sqrt(
														pow(
																prev_img->at<
																		float>(
																		row_prev_cursor
																				- 1,
																		col_prev_cursor
																				+ 1)
																		- img->at<
																				float>(
																				row_prev_cursor
																						- 1,
																				col_prev_cursor
																						+ 1),
																2))
												+ sqrt(
														pow(
																prev_img->at<
																		float>(
																		row_prev_cursor,
																		col_prev_cursor
																				+ 1)
																		- img->at<
																				float>(
																				row_prev_cursor,
																				col_prev_cursor
																						+ 1),
																2))
												+ sqrt(
														pow(
																prev_img->at<
																		float>(
																		row_prev_cursor
																				+ 1,
																		col_prev_cursor
																				+ 1)
																		- img->at<
																				float>(
																				row_prev_cursor
																						+ 1,
																				col_prev_cursor
																						+ 1),
																2));
								if (difference <= min_difference) {
									min_difference = difference;
									min_similarity_row = row_prev_cursor;
									min_similarity_col = col_prev_cursor;
								}
							}
						}
					}



				}

				//on ajoute ce point d'interet a la table de vote
				if (!(min_similarity_col == -1 || min_similarity_row == -1)) {
					//dessin dans l'image prev
#if GRAPHIC ==  1
					cv::arrowedLine(*img_res, cv::Point(col_cursor, row_cursor),cv::Point(cols + min_similarity_col,  min_similarity_row) ,  cv::Scalar(0,0,255));
#endif


					int res = speed_vector_table.at<int>(
							row_cursor - min_similarity_row + rows,
							col_cursor - min_similarity_col + cols);
					res++;
					speed_vector_table.at<int>(
												row_cursor - min_similarity_row + rows,
												col_cursor - min_similarity_col + cols) = res;

					if (res >= speed_vector_max) {
//						 std::cout << "update [ " << min_similarity_col - col_cursor + cols << "," << min_similarity_row - row_cursor + rows <<"] old value :" <<  speed_vector_max << "new res :" <<  speed_vector_max << std::endl;

						speed_vector_max = res;
						 poi_winner_row = row_cursor;
						 poi_winner_col= col_cursor;
						 poi_new_winner_row = min_similarity_row;
						 poi_new_winner_col= min_similarity_col;
						speed_vector_max_col = col_cursor - min_similarity_col  ;
						speed_vector_max_row =  row_cursor - min_similarity_row ;
					} // end if le vecteur vitesse trouvé l'emporte

				}else{
//					std::cout << "pas de matching" << std::endl;
				}

			} // end if pas de point trouvé
			res_harris->at<float>(row_cursor, col_cursor) = k;
		}
	}
#if GRAPHIC ==  1
					cv::arrowedLine(*img_res, cv::Point(poi_winner_col, poi_winner_row),cv::Point(cols + poi_new_winner_col,  poi_new_winner_row) ,  cv::Scalar(0,0,255));
#endif
	cv::imshow("speed table", speed_vector_table);
//	std::this_thread::sleep_for(std::chrono::milliseconds(200));

//    std::cout << pt_interet << " " << speed_vector_max << std::endl;
	ux = speed_vector_max_col;
	uy = speed_vector_max_row;


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
