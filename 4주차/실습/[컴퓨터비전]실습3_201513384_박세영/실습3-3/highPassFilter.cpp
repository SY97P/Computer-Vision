/*
	실습 3-3

	High Pass Filter 구현

	1. 문제 
		- "tibetfox.bmp" 파일에 HPF 구현하여 적용한 후 "tibetfox_hpf.bmp" 로 저장

	2. 요구 결과
		- "tibetfox_hpf.bmp"와 highPassFilter.cpp 파일
	3. 주의
		- 필터 적용시 cv::filter2D 사용 말고 element-wise product
		- 필터 정의시 일반 배열로 정의

*/

#include "opencv2/opencv.hpp"

int main()
{
	cv::Mat image, result_image;

	image = cv::imread("tibetfox.bmp", cv::IMREAD_GRAYSCALE);
	image.copyTo(result_image);

	// high pass filter 선언
	int window[9];
	for (int i = 0; i < 9; i++)
		window[i] = -1;
	window[4] = 8;

	// high pass filter 적용
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
	cv::imwrite("tibetfox_hpf.bmp", result_image);
	cv::waitKey(0);

	return 0;
}