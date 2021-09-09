/*
	실습 3-2

	Low Pass Filter

	"salt_pepper.bmp" 원본 파일에 LPF 윈도우 element wise product

	"salt_pepper_lpf.bmp"로 저장

	! 필터 적용시 cv:filter2D 사용 말고 element-wise product
	! 필터 정의 시 일반 배열로 정의

*/

#include "opencv2/opencv.hpp"

int main()
{
	cv::Mat image, result_image;

	image = cv::imread("salt_pepper.bmp", cv::IMREAD_GRAYSCALE);
	image.copyTo(result_image);

	// Low Pass Filter 윈도우 선언 1/9
	float coef = 1 / (float)9;
	float window[9];
	for (int i = 0; i < 9; i++)
		window[i] = coef;

	// Low Pass Filter 적용
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			if (y == 0 || y == image.rows - 1 || x == 0 || x == image.cols - 1)
				result_image.at<uchar>(y, x) = cv::saturate_cast<uchar>(0);
			else {
				int index = 0;
				int value = 0;
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						value += image.at<uchar>(y + i, x + j) * window[index];
						index++;
					}
				}
				if (value > 255)
					value = 255;
				else if (value < 0)
					value = 0;

				result_image.at<uchar>(y, x) = cv::saturate_cast<uchar>(value);
			}
		}
	}

	cv::imshow("Original Image", image);
	cv::imshow("Result Image", result_image);
	cv::imwrite("salt_pepper_lpf.bmp", result_image);
	cv::waitKey(0);
	std::cout << "salt_pepper filtering done" << std::endl;
}