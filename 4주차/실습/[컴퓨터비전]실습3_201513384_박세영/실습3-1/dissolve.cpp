/*
	실습 3-1
	
	Dissolve 구현

	Fout(j, i) = a * f1(j, i) + (1 - a) * f2(j, i)

	a = 0.3 일 때는 dissolve_3.bmp로 저장
	a = 0.7 일 때는 dissolve_7.bmp로 저장

	둘의 순서에 주의하여 저장

	실시간으로 움직이는 시퀀스는 구현하지 않아도 괜찮음

	dissolve_3.bmp, dissolve_7.bmp, dissolve.cpp 파일 제출

*/

#include "opencv2/opencv.hpp"

int main()
{
	// 이미지를 저장할 변수 선언 (원본 2개, 결과 2개)
	cv::Mat imageCat, imageFox, result_imageD3, result_imageD7;

	// cat.bmp 파일을 읽어 할당한다.
	imageCat = cv::imread("cat.bmp", cv::IMREAD_COLOR);
	imageFox = cv::imread("tibetfox.bmp", cv::IMREAD_COLOR);
	imageCat.copyTo(result_imageD3);
	imageCat.copyTo(result_imageD7);

	// a = 0.3 일 때 result_imageD3 계산
	float a = 0.3;
	for (int y = 0; y < imageCat.rows; y++)
	{
		for (int x = 0; x < imageCat.cols; x++)
		{
			// 원본(고양이, 티벳여우)의 각 픽셀의 BGR 값으로 각각 dissolving
			float b = a * imageCat.at<cv::Vec3b>(y, x)[0] + (1 - a) * imageFox.at<cv::Vec3b>(y, x)[0];
			float g = a * imageCat.at<cv::Vec3b>(y, x)[1] + (1 - a) * imageFox.at<cv::Vec3b>(y, x)[1];
			float r = a * imageCat.at<cv::Vec3b>(y, x)[2] + (1 - a) * imageFox.at<cv::Vec3b>(y, x)[2];

			// 결과 값을 0 ~ 255에 맞게 클램핑하여 result_imageD3에 할당
			result_imageD3.at<cv::Vec3b>(y, x)[0] = cv::saturate_cast<uchar>(b);
			result_imageD3.at<cv::Vec3b>(y, x)[1] = cv::saturate_cast<uchar>(g);
			result_imageD3.at<cv::Vec3b>(y, x)[2] = cv::saturate_cast<uchar>(r);
		}
	}
	
	// a = 0.7 일 때 result_imageD7 계산
	a = 0.7;
	for (int y = 0; y < imageCat.rows; y++)
	{
		for (int x = 0; x < imageCat.cols; x++)
		{
			float b = a * imageCat.at<cv::Vec3b>(y, x)[0] + (1 - a) * imageFox.at<cv::Vec3b>(y, x)[0];
			float g = a * imageCat.at<cv::Vec3b>(y, x)[1] + (1 - a) * imageFox.at<cv::Vec3b>(y, x)[1];
			float r = a * imageCat.at<cv::Vec3b>(y, x)[2] + (1 - a) * imageFox.at<cv::Vec3b>(y, x)[2];

			result_imageD7.at<cv::Vec3b>(y, x)[0] = cv::saturate_cast<uchar>(b);
			result_imageD7.at<cv::Vec3b>(y, x)[1] = cv::saturate_cast<uchar>(g);
			result_imageD7.at<cv::Vec3b>(y, x)[2] = cv::saturate_cast<uchar>(r);
		}
	}

	// dissolve_3 결과 내기
	cv::imshow("Result Image_3", result_imageD3);
	cv::imwrite("dissolve_3.bmp", result_imageD3);
	std::cout << "dissolve_3.bmp 파일 저장함" << std::endl;

	// dissolve_7 결과 내기
	cv::imshow("Result Image_7", result_imageD7);
	cv::imwrite("dissolve_7.bmp", result_imageD7);
	std::cout << "dissolve_7.bmp 파일 저장함" << std::endl;
	cv::waitKey(0);

	return 0;
}