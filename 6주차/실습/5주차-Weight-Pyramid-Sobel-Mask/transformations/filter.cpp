/*
 * 과제5: Weight Pyramid and Sobel Mask
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
	cv::Mat input_mat = input_image.getMat();
	output_image.create(input_mat.size(), input_mat.type());
	cv::Mat output_mat = output_image.getMat();

	int mask_size_half = maskSize / 2;
	for (int y = mask_size_half; y < input_mat.rows - mask_size_half; y++)
	{
		for (int x = mask_size_half; x < input_mat.cols - mask_size_half; x++)
		{
			double sum = 0;
			for (int mask_y = -mask_size_half; mask_y <= mask_size_half; mask_y++)
			{
				for (int mask_x = -mask_size_half; mask_x <= mask_size_half; mask_x++)
				{
					sum += input_mat.at<uchar>(y + mask_y, x + mask_x) * mask[mask_y + mask_size_half][mask_x + mask_size_half];
				}
			}
			output_mat.at<uchar>(y, x) = cv::saturate_cast<uchar>(sum);
		}
	}
}


void SobelFilterY::CreateMask()
{
	maskSize = 3;

	AllocateMask();

	/**
	 * Y방향 Sobel Filter를 구현합니다.
	 * mask 변수에 Sobel Filter Mask를 할당합니다.
	 */
	 // ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
	mask[0][0] = -1;
	mask[0][1] = -2;
	mask[0][2] = -1;
	mask[1][0] = 0;
	mask[1][1] = 0;
	mask[1][2] = 0;
	mask[2][0] = 1;
	mask[2][1] = 2;
	mask[2][2] = 1;
	// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
}

void SobelFilterX::CreateMask()
{
	maskSize = 3;

	AllocateMask();

	/**
	 * X방향 Sobel Filter를 구현합니다.
	 * mask 변수에 Sobel Filter Mask를 할당합니다.
	 */
	// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
	mask[0][0] = -1;
	mask[0][1] = 0;
	mask[0][2] = 1;
	mask[1][0] = -2;
	mask[1][1] = 0;
	mask[1][2] = 2;
	mask[2][0] = -1;
	mask[2][1] = 0;
	mask[2][2] = 1;
	// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
}