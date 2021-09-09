/*
 * 과제9: LOG (Laplacian of Gaussian)
 */

#include "filter.hpp"

Filter::~Filter()
{
	if (mask != nullptr)
	{
		for (int i = 0; i < maskSize; ++i)
		{
			delete[] mask[i];
		}
		delete[] mask;
	}
}

void Filter::AllocateMask()
{
	mask = new double* [maskSize];

	for (int i = 0; i < maskSize; ++i)
	{
		mask[i] = new double[maskSize] { 0.0, };
	}
}

void Filter::Convolute(cv::InputArray& input_image, cv::OutputArray& output_image)
{
	output_image.create(input_image.size(), CV_32FC1);

	cv::Mat input_mat = input_image.getMat();
	cv::Mat output_mat = output_image.getMat();

	int halfSize = maskSize / 2;
	for (int y = halfSize; y < input_mat.rows - halfSize; y++)
	{
		for (int x = halfSize; x < input_mat.cols - halfSize; x++)
		{
			double sum = 0;

			for (int dy = -halfSize; dy <= halfSize; ++dy)
				for (int dx = -halfSize; dx <= halfSize; ++dx)
					sum += input_mat.at<uchar>(y + dy, x + dx) * mask[dy + halfSize][dx + halfSize];

			output_mat.at<float>(y, x) = sum;
		}
	}
}

void LOG::CreateMask()
{
	CreateMask(2.0);
}

void LOG::CreateMask(double sigma)
{
	maskSize = 6 * sigma;
	if (maskSize % 2 == 0)
		++maskSize;

	AllocateMask();

	/**
	 * LOG 필터를 생성합니다.
	 * 주어진 sigma 값에 따른 maskSize * maskSize 크기의 필터를 구현합니다.
	 * 구하는 방법은 PPT와 교재 p.124~130을 참조하세요.
	 */
	// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

	// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
}

void LOG::FindZeroCrossing(cv::InputArray& input_image, cv::OutputArray& output_image)
{
	output_image.create(input_image.size(), CV_8UC1);

	cv::Mat input_mat = input_image.getMat();
	cv::Mat output_mat = output_image.getMat();
	output_mat.setTo(cv::Scalar(0));

	/**
	 * 영교차 알고리즘을 구현하세요.
	 * 주어진 n0와 같이, n1~n7까지를 구현하고
	 * 부호를 확인하여 cnt값을 증가시킵니다.
	 * 
	 * cnt값의 임계치를 2로 두고, 임계치 이상인 경우 영상 output_mat에 255를 할당하세요.
	 */
	// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

	// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
}

int LOG::CheckSign(const float& value)
{
	/**
	 * value의 부호를 결정하는 코드를 작성하세요.
	 * 값이 0인 경우도, 0으로 return하도록 하세요.
	 */
	// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

	// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
}