#include "opencv2/opencv.hpp"

using namespace cv;

int main()
{
	// 이미지를 저장할 변수를 선언한다.
	cv::Mat image, result_image;

	// cat.bmp 파일을 읽어 할당한다. 
	// result_image에도 같은 내용을 할당한다. (원본 파일 보호)
	image = cv::imread("cat.bmp", cv::IMREAD_GRAYSCALE);
	image.copyTo(result_image);

	// 이미지 전체 픽셀을 128을 기준으로 이진화한다. 
	// if(픽셀값 < 128) 결과값 = 0; else 결과값 = 255;
	for(int y = 0; y < image.rows; y++)
	{
		for(int x = 0; x < image.cols; x++)
		{
			// image로부터 grayscale 값을 읽어 바로 이진화한다. 
			int value = image.at<uchar>(y, x);
			int result = 0;
			if (value < 128)
				result = 0;
			else
				result = 255;
			result_image.at<uchar>(y, x) = cv::saturate_cast<uchar>(result);
		}
	}

	cv::imshow("Original Image", image);
	cv::imshow("Result Image", result_image);
	imwrite("cat_binarized.jpg", result_image);
	cv::waitKey(0);

	return 0;
}