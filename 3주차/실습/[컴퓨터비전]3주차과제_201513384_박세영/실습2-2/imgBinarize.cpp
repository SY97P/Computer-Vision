#include "opencv2/opencv.hpp"

using namespace cv;

int main()
{
	// �̹����� ������ ������ �����Ѵ�.
	cv::Mat image, result_image;

	// cat.bmp ������ �о� �Ҵ��Ѵ�. 
	// result_image���� ���� ������ �Ҵ��Ѵ�. (���� ���� ��ȣ)
	image = cv::imread("cat.bmp", cv::IMREAD_GRAYSCALE);
	image.copyTo(result_image);

	// �̹��� ��ü �ȼ��� 128�� �������� ����ȭ�Ѵ�. 
	// if(�ȼ��� < 128) ����� = 0; else ����� = 255;
	for(int y = 0; y < image.rows; y++)
	{
		for(int x = 0; x < image.cols; x++)
		{
			// image�κ��� grayscale ���� �о� �ٷ� ����ȭ�Ѵ�. 
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