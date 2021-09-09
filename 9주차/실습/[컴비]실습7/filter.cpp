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
	
	double sigmaSquare = 2 * maskSize / 6 * maskSize / 6;

	int halfSize = maskSize / 2;
	for (int dy = -halfSize; dy <= halfSize; ++dy) {
		for (int dx = -halfSize; dx <= halfSize; ++dx) {
			// (y^2 + x^2 - 2sigma^2) / sigma^4
			double coef = (dy * dy + dx * dx - sigmaSquare) / (sigmaSquare * sigmaSquare / 4);
			
			// G(y, x)
			double numerator = exp(-((dy * dy + dx * dx) / sigmaSquare));
			double denominator = 3.14 * sigmaSquare;
			mask[halfSize + dy][halfSize + dx] = coef * numerator / denominator;
			//printf("mask = %f\n", mask[halfSize + dy][halfSize + dx]);
		}
	}
	/*
	double sum = 0.0;
	for (int dy = -halfSize; dy <= halfSize; ++dy) {
		for (int dx = -halfSize; dx <= halfSize; ++dx) {
			printf("%f	", mask[halfSize + dy][halfSize + dx]);
			sum += mask[halfSize + dy][halfSize + dx];
		}
		printf("\n");
	}
	printf("%f\n", sum);
	*/
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
	for (int y = 1; y < input_mat.rows - 1; ++y)
	{
		for (int x = 1; x < input_mat.cols - 1; ++x)
		{
			int cnt = 0;
			float n0 = input_mat.at<float>(y, x + 1);
			float n1 = input_mat.at<float>(y + 1, x + 1);
			float n2 = input_mat.at<float>(y + 1, x);
			float n3 = input_mat.at<float>(y + 1, x - 1);
			float n4 = input_mat.at<float>(y, x - 1);
			float n5 = input_mat.at<float>(y - 1, x - 1);
			float n6 = input_mat.at<float>(y - 1, x);
			float n7 = input_mat.at<float>(y - 1, x + 1);

			// 양수-양수 = 1-1
			// 음수-음수 = 0-0
			// 양수-음수 = 1-0
			if (CheckSign(n0) != CheckSign(n4))
				cnt++;
			if (CheckSign(n1) != CheckSign(n5))
				cnt++;
			if (CheckSign(n2) != CheckSign(n6))
				cnt++;
			if (CheckSign(n3) != CheckSign(n7))
				cnt++;

			if (cnt >= 2)
				output_mat.at<uchar>(y, x) = 255;
		}
	}
	// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
}

int LOG::CheckSign(const float& value)
{
	/**
	 * value의 부호를 결정하는 코드를 작성하세요.
	 * 값이 0인 경우도, 0으로 return하도록 하세요.
	 */
	// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

	// 양수이면 1
	// 음수나 0이면 0
	if (value > 0)
		return 1;
	else
		return 0;
	// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
}