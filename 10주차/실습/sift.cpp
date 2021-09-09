﻿/*
 * 과제10: SIFT (Scale-Invariant Feature Transform)
 */

#include "sift.hpp"
#include <string>

using namespace algorithms;

//
// 옥타브 세트를 만들어 반환한다.
// OctaveSet은 8개의 Gaussian Image와 DoG로 이루어진 하나의 세트이다. (교재 p.189 참조)
// 
OctaveSet SIFT::GenerateOctaveSet(const cv::InputArray& octave_original_image)
{
	cv::Mat input_mat = octave_original_image.getMat();

	// 첫번째 영상은 스무딩이 적용되었다고 가정함
	OctaveSet octave_set;
	memset(&octave_set, 0, sizeof(OctaveSet));

	octave_set.id = octave_index_count++;
	input_mat.copyTo(octave_set.gaussianMat[0]);

	// 첫번째 이외의 이미지에 해당하는 sigma값 만큼 가우시안 적용 후 octave_set.gaussianMat 배열에 저장
	// (아래 API를 이용하여 코드를 작성합니다.)
	/*
		void GaussianBlur(InputArray src, OutputArray dst, Size ksize, double sigmaX);
		@src 원본 영상(input_mat)
		@dst 결과 영상(octave_set.gaussianMat 배열)
		@ksize 커널 사이즈(cv::Size(0, 0))
		@sigmaX sigma(octave_sigma_values 배열)
	*/
	for (int i = 1; i < 6; ++i)
	{
		// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
		cv::GaussianBlur(input_mat, octave_set.gaussianMat[i], cv::Size(0, 0), octave_sigma_values[i]);

		// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
	}

	// Gaussian 이미지를 이용하여 DoG (Derivative of Gaussian)을 구한다.
	for (int i = 1; i < 6; ++i)
	{
		// octave_set.gaussianMat 가우시안 이미지를 이용하여,
		// octave_set.gaussianDerivativeMat에 DoG를 생성하세요.
		// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

		// octave_set.gaussianMat[0] : 스무딩이 되었다고 가정한 원본 이미지
		// octave_set.gaussianMat[1..5] : sigma 값을 다르게 하여 원본 이미지를 가우시안 필터링(스무딩)한 영상
		// DoG(sigma_i) = (G(k*sigma_i) - G(sigma_i)) * f 를 이용하면 될 것 같음. k = 2^(1/3) 이라고 되어있음.
		// 이게 아님.
		// 위에서 gaussianBlur로 구한 octave_set.gaussianMat이 원 영상에 gaussain filtering이 이미 된 거임 다시 말해서 G(sigma_i) * f 인거
		// 따라서 DoG(sigma_i) = G(sigma_i+1) * f - G(sigma_i) * f 로 구해야함. 

		// gaussainDerivativeMat은 공간 할당이 되어 있지 않았음.
		// 그러니 copyTo로 DoG(i) = G(i) * f 를 먼저 해주고
		// 그 다음에 DoG(i) -= G(i-1) * f 를 해주면 될 것 같음.
		octave_set.gaussianMat[i].copyTo(octave_set.gaussianDerivativeMat[i-1]);
		
		for (int y = 0; y < octave_set.gaussianDerivativeMat[i-1].rows; y++)
		{
			for (int x = 0; x < octave_set.gaussianDerivativeMat[i-1].cols; x++)
			{
				int value = octave_set.gaussianDerivativeMat[i - 1].at<uchar>(y, x);
				value -= octave_set.gaussianMat[i - 1].at<uchar>(y, x);
				if (value < 0)
					value = -1 * value;
				octave_set.gaussianDerivativeMat[i - 1].at<uchar>(y, x) = value;
			}
		}

		// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **
	}

	// 만들어진 옥타브 세트를 반환한다.
	return octave_set;
}

//
std::vector<KeyPoint> SIFT::FindKeyPoints(const OctaveSet& octave_set)
{
	std::vector<KeyPoint> key_points;

	for (int octave_idx = 1; octave_idx <= 3; ++octave_idx)
		for (int h = 1; h < octave_set.gaussianDerivativeMat[octave_idx].rows - 1; ++h)
			for (int w = 1; w < octave_set.gaussianDerivativeMat[octave_idx].cols - 1; ++w)
			{
				// 특징점의 값에 문턱값(Threshold)을 적용하여
				// 너무 작은 값은 배제
				if (octave_set.gaussianDerivativeMat[octave_idx].at<uchar>(h, w) <= 8)
					continue;

				int max_v = octave_set.gaussianDerivativeMat[octave_idx].at<uchar>(h, w);
				int min_v = octave_set.gaussianDerivativeMat[octave_idx].at<uchar>(h, w);

				for (int o2 = octave_idx - 1; o2 <= octave_idx + 1; ++o2)
					for (int h2 = h - 1; h2 <= h + 1; ++h2)
						for (int w2 = w - 1; w2 <= w + 1; ++w2)
						{
							if (octave_idx == o2 && h == h2 && w == w2)
								continue;

							if (octave_set.gaussianDerivativeMat[o2].at<uchar>(h2, w2) > max_v)
								max_v = octave_set.gaussianDerivativeMat[o2].at<uchar>(h2, w2);

							if (octave_set.gaussianDerivativeMat[o2].at<uchar>(h2, w2) < min_v)
								min_v = octave_set.gaussianDerivativeMat[o2].at<uchar>(h2, w2);
						}
				//최댓, 최솟값이 현재 좌표이면 키포인트(특징점)으로 판단
				if (max_v == octave_set.gaussianDerivativeMat[octave_idx].at<uchar>(h, w) ||
					min_v == octave_set.gaussianDerivativeMat[octave_idx].at<uchar>(h, w))
					key_points.push_back(KeyPoint{ h, w, octave_sigma_values[octave_idx], 0., octave_set.id });
			}

	return key_points;
}

void SIFT::DrawKeyPoints(const std::vector<KeyPoint>& key_points, const cv::InputArray& original_image, const cv::OutputArray& drawn_image)
{
	if (original_image.channels() == 1)
		cv::cvtColor(original_image, drawn_image, cv::COLOR_GRAY2BGR);
	else
		original_image.copyTo(drawn_image);

	cv::Mat drawn_mat = drawn_image.getMat();
	for (int i = 0; i < key_points.size(); ++i)
	{
		int scale = pow(2, key_points[i].octave);
		cv::circle(drawn_mat, cv::Point(key_points[i].x * scale, key_points[i].y) * scale, 3, cv::Scalar(255, 0, 0));
	}
}


void SIFT::FindDominantOrientation(std::vector<KeyPoint>& key_points, std::vector<OctaveSet>& octave_sets)
{
	// 에지 히스토그램 개수
	const int SIFT_ORI_HIST_BINS = 36;
	for (int i = 0; i < key_points.size(); ++i)
	{
		//printf("i = %d\n", i);
		KeyPoint& key_point = key_points[i];

		/*
			교재 p.256
			Hess가 구현한 방법에서는 1.5 x 3 x Sigma를 반올림하여 w를 구한 후
			(2w+1) x (2w+1)의 윈도우를 사용하였다.
		*/
		int radius = 1.5 * 3 * key_point.sigma;

		std::vector<double> angles;
		std::vector<double> magnitudes;

		// Octave와 Sigma에 맞는 영상으로 처리
		int sigmaIdx = GetOctaveIdx(key_point.sigma);
		assert((0 <= sigmaIdx) && (sigmaIdx < 6));
		cv::Mat img = octave_sets[key_point.octave].gaussianMat[sigmaIdx];
		// radius : 영상 너비, 높이
		for (int h = -radius; h <= radius; ++h)
		{
			int y = key_point.y + h;
			if (y <= 0 || y >= img.rows - 1)
				continue;
			for (int w = -radius; w <= radius; w++)
			{
				int x = key_point.x + w;
				if (x <= 0 || x >= img.cols - 1)
					continue;

				// 픽셀별 dx와 dy를 계산하시오.
				// 각각 좌우 픽셀 변화량, 상하 픽셀 변화량을 계산하세요.
				// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

				float dx = -1 * img.at<uchar>(y, x - 1) + 1 * img.at<uchar>(y, x + 1);
				float dy = -1 * img.at<uchar>(y - 1, x) + 1 * img.at<uchar>(y + 1, x);

				//printf("(y, x-1) = %d\n", img.at<uchar>(y, x - 1));
				//printf("(y, x+1) = %d\n", img.at<uchar>(y, x + 1));

				//printf("dx = %f\n", dx);


				//printf("(y-1, x) = %d\n", img.at<uchar>(y-1, x ));
				//printf("(y+1, x) = %d\n", img.at<uchar>(y+1, x));

				//printf("dy = %f\n", dy);

				// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

				cv::Mat magnitude_map(cv::Size(1, 1), CV_64FC1);
				cv::Mat angle_map(cv::Size(1, 1), CV_64FC1);

				/*
					void cartToPolar(InputArray x, InputArray y,
							  OutputArray magnitude, OutputArray angle,
							  bool angleInDegrees = false);
					@x dx
					@y dy
					@magnitude 에지 크기
					@angle 에지 방향
					@angleInDegrees true일 경우 degree (여기서는 true)
				*/
				// 에지 방향과 에지 크기를 계산하시오.
				// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

				// 이렇게 하면 output인데 2차원 array로 magnitude랑 angle이 구해지는 것 같음
				// angle_map의 0,0에 있는 double 값을 angles라는 벡터로 변환함.
				// 근데 왜 0,0인지는 모르겠음.
				cv::cartToPolar(dx, dy, magnitude_map, angle_map, true);

				// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

				angles.push_back(angle_map.at<double>(0, 0));
				magnitudes.push_back(magnitude_map.at<double>(0, 0));
			}
		}

		float hist[SIFT_ORI_HIST_BINS] = { 0, };
		/*for (int index = 0; index < 36; index++)
			printf("%f ", hist[index]);*/
		//printf("\n");
		for (int s = 0; s < angles.size(); ++s) // also iterates magnitude
		{
			// bin = 10도 간격으로 양자화된 에지 방향 값
			// cvRound 함수와 SIFT_ORI_HIST_BINS 상수를 이용하여 bin값을 반올림하여 구하세요.
			// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

			// bin : 0~36의 양자화된 orientation
			// hist : bin값을 index로 해서 freq-orient 차원의 히스토그램
			// 아래 코드에서 bin값을 양자화 한 결과를 구해주기 때문에 
			// 여기서는 cvRound(double) 함수를 이용해서 bin의 값만 반올림해서 초기화해주면 될 것 같음.
			int bin = cvRound(angles[s]/10);

			// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

			if (bin >= SIFT_ORI_HIST_BINS)
				bin -= SIFT_ORI_HIST_BINS;
			if (bin < 0)
				bin += SIFT_ORI_HIST_BINS;

			//printf("bin = %d\n", bin);
			//printf("hist[bin] = %f\n", hist[bin]);
			//printf("manitudes[s] = %f\n", magnitudes[s]);

			// 히스토그램의 값을 에지의 방향과 강도의 곱으로 계산
			hist[bin] += (1 * magnitudes[s]);

			//printf("hist[bin] = %f\n", hist[bin]);
		}

		float maxval = hist[0];
		float maxOri = 0;
		// 값이 최대인 히스토그램 값을 Dominant Orienation으로 설정
		for (int j = 1; j < SIFT_ORI_HIST_BINS; j++) {
			if (maxval < hist[j])
			{
				maxval = hist[j];
				maxOri = j;
			}
		}
		key_point.dominantOrientation = maxOri;
	}
}

std::vector<DescriptorVector> SIFT::MakeDescriptor(std::vector<KeyPoint>& key_points, std::vector<OctaveSet>& octave_sets)
{
	std::vector<DescriptorVector> descriptor_vectors;

	// 에지 히스토그램 개수
	const int SIFT_ORI_HIST_BINS = 8;
	for (int i = 0; i < key_points.size(); ++i)
	{
		KeyPoint& key = key_points[i];
		/*
			교재 p.257
			Hess가 구현한 방법에서는 siz를 3 x sigma로 설정하였다.
		*/
		int siz = 3 * key.sigma;

		// 36개로 양자화하여 10도씩 에지 히스토그램을 만들어기 때문에
		// 원래 에지를 계산하기 위해서 10을 곱해줌
		float orientation = key.dominantOrientation * 10;

		cv::Point beginPoint = cv::Point(key.x - (2 * siz), key.y - (2 * siz));
		cv::Point endPoint = cv::Point(key.x + (2 * siz), key.y + (2 * siz));

		// Octave와 Sigma에 맞는 영상으로 처리
		int sigmaIdx = GetOctaveIdx(key.sigma);
		const cv::Mat& img = octave_sets[key.octave].gaussianMat[sigmaIdx];

		// 중점에서 회전하는 복합 행렬을 구하시오.
		/*
			getRotationMatrix2D(Point2f center, double angle, double scale);
			@center 중점 (cv::Point). 예를 들어, cv::Point(centerX, centerY)
			@angle 회전 방향
			@scale 스케일 (실습에서는 1)
		*/
		// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

		// 원점이 아니라 keypoint(key변수)에 해당하는 점을 중점으로 해서 동차행렬을 구해야 할 거 같음
		cv::Mat m = cv::getRotationMatrix2D(cv::Point(key.x, key.y), orientation, 1);

		// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

		for (int h = beginPoint.y; h < endPoint.y; h += siz)
		{
			for (int w = beginPoint.x; w < endPoint.x; w += siz)
			{
				std::vector<double> angles;
				std::vector<double> magnitudes;
				for (int h2 = h; h2 < h + siz; h2++)
				{
					for (int w2 = w; w2 < w + siz; w2++)
					{
						cv::Mat orgIdx(cv::Size(1, 2), CV_64FC1);

						orgIdx.at<double>(0) = h2;
						orgIdx.at<double>(1) = w2;

						// 복합 동차 행렬 계산
						int rotationIdxY = int(orgIdx.at<double>(0) * m.at<double>(0) + orgIdx.at<double>(1) * m.at<double>(1) + m.at<double>(2));
						int rotationIdxX = int(orgIdx.at<double>(0) * m.at<double>(3) + orgIdx.at<double>(1) * m.at<double>(4) + m.at<double>(5));

						if (rotationIdxX > 0 && rotationIdxX < img.cols - 1 && rotationIdxY > 0 && rotationIdxY < img.rows - 1)
						{
							// 픽셀별 dx와 dy를 계산하시오.(rotationIdxY와 rotationIdxX를 이용하세요.)
							// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

							float dx = -1 * img.at<uchar>(rotationIdxY, rotationIdxX - 1) + 1 * img.at<uchar>(rotationIdxY, rotationIdxX + 1);
							float dy = -1 * img.at<uchar>(rotationIdxY - 1, rotationIdxX) + 1 * img.at<uchar>(rotationIdxY + 1, rotationIdxX);

							// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

							cv::Mat magnitude_map(cv::Size(1, 1), CV_64FC1);
							cv::Mat angle_map(cv::Size(1, 1), CV_64FC1);

							/*
								void cartToPolar(InputArray x, InputArray y,
										  OutputArray magnitude, OutputArray angle,
										  bool angleInDegrees = false);
								@x dx
								@y dy
								@magnitude 에지 크기
								@angle 에지 방향
								@angleInDegrees true일 경우 degree (여기서는 true)
							*/
							// 에지 방향과 에지 크기를 계산하시오.
							// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

							cv::cartToPolar(dx, dy, magnitude_map, angle_map, true);

							// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

							angles.push_back(angle_map.at<double>(0));
							magnitudes.push_back(magnitude_map.at<double>(0));
						}
					}
				}

				DescriptorVector vector;
				memset(&vector, 0, sizeof(DescriptorVector));

				for (int s = 0; s < angles.size(); ++s)
				{
					// bin = 8단계로 양자화된 에지 방향 값.
					// cvRound 함수와 SIFT_ORI_HIST_BINS 상수를 이용하여 bin값을 반올림하여 구하세요.
					// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

					int bin = cvRound(angles[s]/45);

					// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

					if (bin >= SIFT_ORI_HIST_BINS)
						bin -= SIFT_ORI_HIST_BINS;
					if (bin < 0)
						bin += SIFT_ORI_HIST_BINS;

					vector.descriptor[bin] += (1 * magnitudes[s]);
				}

				float sum_of_histogram = 0;

				// 정규화 구현
				// Descriptor 값들에 대한 정규화를 진행합니다.
				// SIFT_ORI_HIST_BINS개의 히스토그램 값 vector.descriptor들에 대해,
				// 모든 값을 합한 값 sum_of_histogram으로 각 element를 나누어줍니다.

				// ** 지금부터 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

				// Descriptor(vector.descriptor)는 float 타입의 벡터
				// sum_of_histogram을 float로 해서 구하자.
				for (int index = 0; index < SIFT_ORI_HIST_BINS; index++)
				{
					sum_of_histogram += vector.descriptor[index];
				}
				// sumof로 element를 나눠서 갱신
				for (int i = 0; i < SIFT_ORI_HIST_BINS; i++)
				{
					if (sum_of_histogram != 0)
						vector.descriptor[i] /= sum_of_histogram;
					else
						vector.descriptor[i] = 0;
				}

				// ** 여기까지 코드를 작성하세요. 이 줄은 지우시면 안 됩니다 **

				descriptor_vectors.push_back(vector);
			}
		}
	}

	return descriptor_vectors;
}

int SIFT::GetOctaveIdx(const double& sigma)
{
	double epsilon = 1e-6;

	for (int i = 0; i < 6; ++i)
		if (sigma == octave_sigma_values[i]) // roughly sigma == sig[i]
			return i;

	// Failed to find appropriate sigma value!
	assert(false);
	return -1;
}