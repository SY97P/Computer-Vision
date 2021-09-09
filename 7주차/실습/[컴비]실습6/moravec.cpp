#include <algorithm>
#include "moravec.hpp"

Moravec::Moravec(cv::InputArray& original_image)
{
	cv::Mat src = original_image.getMat();
	confidence_map.create(src.size(), CV_32FC1);

	// �ܰ� 2�� �ȼ��� Confidence�� 0���� ����
	for (int w = 0; w < src.cols; ++w)
	{
		confidence_map.at<float>(0, w) = 0.0;
		confidence_map.at<float>(1, w) = 0.0;
		confidence_map.at<float>(src.rows - 1, w) = 0.0;
		confidence_map.at<float>(src.rows - 2, w) = 0.0;
	}

	for (int h = 0; h < src.rows; ++h)
	{
		confidence_map.at<float>(h, 0) = 0.0;
		confidence_map.at<float>(h, 1) = 0.0;
		confidence_map.at<float>(h, src.cols - 1) = 0.0;
		confidence_map.at<float>(h, src.cols - 2) = 0.0;
	}
}


void Moravec::FindConfidenceMap(cv::InputArray& original_image)
{
	cv::Mat src = original_image.getMat();
	
	/**
	 * src�� �̿��Ͽ� ��� �˰����� Confidence�� ����� ��
	 * �̸� confidence_map�� �����ϼ���.
	 * (confidence_map�� Ŭ���� ��� ������ ����Ǿ� �ֽ��ϴ�.)
	 */
	// ** ���ݺ��� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
	
	for (int y = 2; y < src.rows - 2; y++) {
		for (int x = 2; x < src.cols - 2; x++) {
			int curr[9];
			int smap[4][9];
			int index = 0;
			for (int j = -1; j < 2; j++) {
				for (int i = -1; i < 2; i++) {
					curr[index] = src.at<uchar>(y + j, x + i);
					smap[0][index] = src.at<uchar>(y - 1 + j, x + i);
					smap[1][index] = src.at<uchar>(y + 1 + j, x + i);
					smap[2][index] = src.at<uchar>(y + j, x - 1 + i);
					smap[3][index] = src.at<uchar>(y + j, x + 1 + i);
					index++;
				}
			}
			
			int svalue[4];
			for (int i = 0; i < 4; i++) {
				svalue[i] = 0;
				for (int j = 0; j < 9; j++) {
					svalue[i] += pow(curr[j] - smap[i][j], 2);
				}
			}
			/*
			for (int i = 0; i < 4; i++) {
				if (svalue[i] > 15000)
					printf("%d ", svalue[i]);
			}
			*/
			
			int min = svalue[0];
			for (int i = 1; i < 4; i++) {
				if (min > svalue[i])
					min = svalue[i];
			}
			confidence_map.at<float>(y, x) = min;
			
			/*
			if (y == 2 && x == 2) {
				printf("curr\n");
				for (int i = 0; i < 9; i++) {
					printf("%d ", curr[i]);
					if (i % 3 == 2)
						printf("\n");
				}
				printf("\ns(-1, 0)\n");
				for (int i = 0; i < 9; i++) {
					printf("%d ", smap[0][i]);
					if (i % 3 == 2)
						printf("\n");
				}
				printf("\ns(1, 0)\n");
				for (int i = 0; i < 9; i++) {
					printf("%d ", smap[1][i]);
					if (i % 3 == 2)
						printf("\n");
				}
				printf("\ns(0, -1)\n");
				for (int i = 0; i < 9; i++) {
					printf("%d ", smap[2][i]);
					if (i % 3 == 2)
						printf("\n");
				}
				printf("\ns(0, 1)\n");
				for (int i = 0; i < 9; i++) {
					printf("%d ", smap[3][i]);
					if (i % 3 == 2)
						printf("\n");
				}
			}
			*/
		}
	}
	// ** ������� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
}

void Moravec::DrawFeature(cv::OutputArray& result_image)
{
	cv::Mat dst = result_image.getMat();

	for (int h = 0; h < confidence_map.rows; ++h)
	{
		for (int w = 0; w < confidence_map.cols; ++w)
		{
			if (confidence_map.at<float>(h, w) >= 15000)
			{
				cv::circle(dst, cv::Point(w, h), 3, cv::Scalar(255, 0, 0));
			}
		}
	}
}