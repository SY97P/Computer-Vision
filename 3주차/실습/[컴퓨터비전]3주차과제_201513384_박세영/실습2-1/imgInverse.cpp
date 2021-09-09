#include "opencv2/opencv.hpp"

using namespace cv;

int main()
{
	// 이미지를 저장할 변수를 선언한다.
	// Mat 객체는 image를 BGR로 표현하는 클래스
	// 원본 파일 보호를 위해 작업은 result_image에서 할거임.
	cv::Mat image, result_image;

	// cat.bmp 파일을 읽어 할당한다.
	image = cv::imread("cat.bmp", cv::IMREAD_COLOR);
	// result_image에도 할당해줌
	image.copyTo(result_image);

	// 이미지를 반전한다. 
	// BGR 모두다 해줘야 한다. 한 픽셀당 세번의 연산
	// 바뀔값 = 255 - 원래 값
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			// 원래값은 원본 image에서 가져오고 작업 결과는 result_image에 저장
			int b = 255 - image.at<Vec3b>(y, x)[0];
			int g = 255 - image.at<Vec3b>(y, x)[1];
			int r = 255 - image.at<Vec3b>(y, x)[2];

			// 픽셀의 BGR 값이 unsigned 범위를 벗어나지 않게 클램핑
			result_image.at<Vec3b>(y, x)[0] = cv::saturate_cast<uchar>(b);
			result_image.at<Vec3b>(y, x)[1] = cv::saturate_cast<uchar>(g);
			result_image.at<Vec3b>(y, x)[2] = cv::saturate_cast<uchar>(r);
		}
	}

	// 이미지를 서로 비교한다. 
	cv::imshow("Original Image", image);
	cv::imshow("Result Image", result_image);
	// 결과 이미지 저장
	imwrite("cat_inverted.jpg", result_image);
	// 아무키나 누르면 종료.
	cv::waitKey(0); 

	/*
	// 디버깅
	// 이미지 최상단 좌측의 픽셀 한 개의 R, G, B 값을 읽고 출력한다. 
	int x = 0, y = 0;
	std::cout << "B: " << (unsigned int)image.at<Vec3b>(y, x)[0] << " ";
	std::cout << "G: " << (unsigned int)image.at<Vec3b>(y, x)[1] << " ";
	std::cout << "R: " << (unsigned int)image.at<Vec3b>(y, x)[2] << " ";
	std::cout << "B: " << (unsigned int)result_image.at<Vec3b>(y, x)[0] << " ";
	std::cout << "G: " << (unsigned int)result_image.at<Vec3b>(y, x)[1] << " ";
	std::cout << "R: " << (unsigned int)result_image.at<Vec3b>(y, x)[2] << " ";
	*/

	return 0;
}