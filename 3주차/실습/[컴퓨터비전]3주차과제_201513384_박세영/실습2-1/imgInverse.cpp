#include "opencv2/opencv.hpp"

using namespace cv;

int main()
{
	// �̹����� ������ ������ �����Ѵ�.
	// Mat ��ü�� image�� BGR�� ǥ���ϴ� Ŭ����
	// ���� ���� ��ȣ�� ���� �۾��� result_image���� �Ұ���.
	cv::Mat image, result_image;

	// cat.bmp ������ �о� �Ҵ��Ѵ�.
	image = cv::imread("cat.bmp", cv::IMREAD_COLOR);
	// result_image���� �Ҵ�����
	image.copyTo(result_image);

	// �̹����� �����Ѵ�. 
	// BGR ��δ� ����� �Ѵ�. �� �ȼ��� ������ ����
	// �ٲ� = 255 - ���� ��
	for (int y = 0; y < image.rows; y++)
	{
		for (int x = 0; x < image.cols; x++)
		{
			// �������� ���� image���� �������� �۾� ����� result_image�� ����
			int b = 255 - image.at<Vec3b>(y, x)[0];
			int g = 255 - image.at<Vec3b>(y, x)[1];
			int r = 255 - image.at<Vec3b>(y, x)[2];

			// �ȼ��� BGR ���� unsigned ������ ����� �ʰ� Ŭ����
			result_image.at<Vec3b>(y, x)[0] = cv::saturate_cast<uchar>(b);
			result_image.at<Vec3b>(y, x)[1] = cv::saturate_cast<uchar>(g);
			result_image.at<Vec3b>(y, x)[2] = cv::saturate_cast<uchar>(r);
		}
	}

	// �̹����� ���� ���Ѵ�. 
	cv::imshow("Original Image", image);
	cv::imshow("Result Image", result_image);
	// ��� �̹��� ����
	imwrite("cat_inverted.jpg", result_image);
	// �ƹ�Ű�� ������ ����.
	cv::waitKey(0); 

	/*
	// �����
	// �̹��� �ֻ�� ������ �ȼ� �� ���� R, G, B ���� �а� ����Ѵ�. 
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