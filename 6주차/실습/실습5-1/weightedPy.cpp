/*
 * 과제5: Weight Pyramid and Sobel Mask
 */

#include "opencv2/opencv.hpp"
#include "transformations/filter.hpp"

void getSimpleDownscaledPyramid(const int Q, const cv::InputArray& input_image, std::vector<cv::Mat>& output_vector)
{
	cv::Mat input_mat = input_image.getMat();
	int width = input_mat.cols;
	int height = input_mat.rows;

	cv::Mat temp;
	input_mat.copyTo(temp);

	for (int q = 0; q < Q; ++q)
	{
		cv::Mat dst(cv::Size(width / 2, height / 2), CV_8UC1);

		for (int h = 0; h < dst.rows; ++h)
		{
			for (int w = 0; w < dst.cols; ++w)
			{
				dst.at<uchar>(h, w) = temp.at<uchar>(h * 2, w * 2);
			}
		}
		output_vector.push_back(dst);
		dst.copyTo(temp);

		width = width / 2;
		height = height / 2;
	}
}

void getWeightPyramid(const int Q, const cv::InputArray& input_image, std::vector<cv::Mat>& output_vector)
{
	cv::Mat input_mat = input_image.getMat();
	int width = input_mat.cols;
	int height = input_mat.rows;

	double weight[5][5] = {
		0.0025, 0.0125, 0.02, 0.0125, 0.0025,
		0.0125, 0.0625, 0.1, 0.0625, 0.0125,
		0.02, 0.1, 0.16, 0.1, 0.02,
		0.0125, 0.0625, 0.1, 0.0625, 0.0125,
		0.0025, 0.0125, 0.02, 0.0125, 0.0025
	};


	/**
	 * Weight Pyramid를 구현합니다.
	 * 위에 주어진 weight kernel을 이용하여 Convolution 연산을 구현합니다.
	 * 구현하여 만들어진 결과 이미지는 output_vector에 저장합니다.
	 *
	 * Q는 결과 피라미드 이미지의 갯수입니다. Q번만큼 다운스케일링을 진행하면 됩니다.
	 */
	 // ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

	cv::Mat temp;
	input_mat.copyTo(temp);
	/*
	// 첫번째 weighted pyramid
	for (int y = 2; y < height - 2; y++) {
		for (int x = 2; x < width - 2; x++) {
			double value = 0;
			for (int j = 0; j < 5; j++) {
				for (int i = 0; i < 5; i++) {
					value += input_mat.at<uchar>(y - 2 + j, x - 2 + i) * weight[j][i];
				}
			}
			int result = (int)value;
			temp.at<uchar>(y, x) = cv::saturate_cast<uchar>(result);
		}
	}
	cv::imshow("first temp", temp);

	// 두번째 weighted pyramid
	cv::Mat temp_f;
	temp.copyTo(temp_f);
	for (int y = 4; y < height - 4; y += 2) {
		for (int x = 4; x < width - 4; x += 2) {
			double value = 0;
			for (int j = 0; j < 5; j++) {
				for (int i = 0; i < 5; i++) {
					value += temp_f.at<uchar>(y - 4 + 2 * j, x - 4 + 2 * i) * weight[j][i];
				}
			}
			int result = (int)value;
			temp.at<uchar>(y, x) = cv::saturate_cast<uchar>(result);
		}
	}
	cv::imshow("second temp", temp);

	// 세번째 weighted pyramid
	temp.copyTo(temp_f);
	for (int y = 8; y < height - 8; y += 4) {
		for (int x = 8; x < width - 8; x += 4) {
			double value = 0;
			for (int j = 0; j < 5; j++) {
				for (int i = 0; i < 5; i++) {
					value += temp_f.at<uchar>(y - 8 + 4 * j, x - 8 + 4 * i) * weight[j][i];
				}
			}
			int result = (int)value;
			temp.at<uchar>(y, x) = cv::saturate_cast<uchar>(result);
		}
	}
	cv::imshow("third temp", temp);



	// 네번째 weighted pyramid
	temp.copyTo(temp_f);
	for (int y = 16; y < height - 16; y += 8) {
		for (int x = 16; x < width - 16; x += 8) {
			double value = 0;
			for (int j = 0; j < 5; j++) {
				for (int i = 0; i < 5; i++) {
					value += temp_f.at<uchar>(y - 16 + 8 * j, x - 16 + 8 * i) * weight[j][i];
				}
			}
			int result = (int)value;
			temp.at<uchar>(y, x) = cv::saturate_cast<uchar>(result);
		}
	}
	cv::imshow("forth temp", temp);
	*/
	cv::Mat temp_f;

	for (int q = 0; q < Q; q++) {
		temp.copyTo(temp_f);
		int quick = pow(2, q + 1);
		for (int y = quick; y < height - quick; y += quick / 2) {
			for (int x = quick; x < width - quick; x += quick / 2) {
				double value = 0;
				for (int j = 0; j < 5; j++) {
					for (int i = 0; i < 5; i++) {
						value += temp_f.at<uchar>(y - quick + quick / 2 * j, x - quick + quick / 2 * i) * weight[j][i];
					}
				}
				int result = (int)value;
				temp.at<uchar>(y, x) = cv::saturate_cast<uchar>(result);
			}
		}
	}


	getSimpleDownscaledPyramid(4, temp, output_vector);
	// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
}

int main()
{
	std::vector<cv::Mat> octave_image;
	cv::Mat original_image;
	original_image = cv::imread("cat.bmp", cv::IMREAD_GRAYSCALE);

	// getSimpleDownscaledPyramid(4, original_image, octave_image);
	getWeightPyramid(4, original_image, octave_image);

	int idx = 0;
	for (auto& v : octave_image)
	{
		std::string str = std::to_string(idx++);
		cv::namedWindow(str.c_str(), cv::WINDOW_NORMAL);
		cv::imshow(str.c_str(), v);
		cv::resizeWindow(str.c_str(), cv::Size(640, 360));

		// 이미지를 파일로 저장한다.
		cv::imwrite("cat_q" + std::to_string(idx - 1) + ".bmp", v);
	}
	cv::waitKey();

}