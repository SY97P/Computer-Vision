/*
	�ǽ� 3-1
	
	Dissolve ����

	Fout(j, i) = a * f1(j, i) + (1 - a) * f2(j, i)

	a = 0.3 �� ���� dissolve_3.bmp�� ����
	a = 0.7 �� ���� dissolve_7.bmp�� ����

	���� ������ �����Ͽ� ����

	�ǽð����� �����̴� �������� �������� �ʾƵ� ������

	dissolve_3.bmp, dissolve_7.bmp, dissolve.cpp ���� ����

*/

#include "opencv2/opencv.hpp"

int main()
{
	// �̹����� ������ ���� ���� (���� 2��, ��� 2��)
	cv::Mat imageCat, imageFox, result_imageD3, result_imageD7;

	// cat.bmp ������ �о� �Ҵ��Ѵ�.
	imageCat = cv::imread("cat.bmp", cv::IMREAD_COLOR);
	imageFox = cv::imread("tibetfox.bmp", cv::IMREAD_COLOR);
	imageCat.copyTo(result_imageD3);
	imageCat.copyTo(result_imageD7);

	// a = 0.3 �� �� result_imageD3 ���
	float a = 0.3;
	for (int y = 0; y < imageCat.rows; y++)
	{
		for (int x = 0; x < imageCat.cols; x++)
		{
			// ����(�����, Ƽ������)�� �� �ȼ��� BGR ������ ���� dissolving
			float b = a * imageCat.at<cv::Vec3b>(y, x)[0] + (1 - a) * imageFox.at<cv::Vec3b>(y, x)[0];
			float g = a * imageCat.at<cv::Vec3b>(y, x)[1] + (1 - a) * imageFox.at<cv::Vec3b>(y, x)[1];
			float r = a * imageCat.at<cv::Vec3b>(y, x)[2] + (1 - a) * imageFox.at<cv::Vec3b>(y, x)[2];

			// ��� ���� 0 ~ 255�� �°� Ŭ�����Ͽ� result_imageD3�� �Ҵ�
			result_imageD3.at<cv::Vec3b>(y, x)[0] = cv::saturate_cast<uchar>(b);
			result_imageD3.at<cv::Vec3b>(y, x)[1] = cv::saturate_cast<uchar>(g);
			result_imageD3.at<cv::Vec3b>(y, x)[2] = cv::saturate_cast<uchar>(r);
		}
	}
	
	// a = 0.7 �� �� result_imageD7 ���
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

	// dissolve_3 ��� ����
	cv::imshow("Result Image_3", result_imageD3);
	cv::imwrite("dissolve_3.bmp", result_imageD3);
	std::cout << "dissolve_3.bmp ���� ������" << std::endl;

	// dissolve_7 ��� ����
	cv::imshow("Result Image_7", result_imageD7);
	cv::imwrite("dissolve_7.bmp", result_imageD7);
	std::cout << "dissolve_7.bmp ���� ������" << std::endl;
	cv::waitKey(0);

	return 0;
}