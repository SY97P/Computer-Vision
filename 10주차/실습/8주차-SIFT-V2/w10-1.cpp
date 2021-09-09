/*
 * 과제10: SIFT (Scale-Invariant Feature Transform)
 */

#include "opencv2/opencv.hpp"
#include "algorithms/sift.hpp"

using namespace algorithms;

int main()
{
	cv::Mat original_image, gaussian_image, keypoint_image;

	original_image = cv::imread("cliff.bmp", cv::IMREAD_GRAYSCALE);
	assert(original_image.rows > 200 && original_image.cols > 200);

	cv::GaussianBlur(original_image, gaussian_image, cv::Size(0, 0), 1.6, 1.6);

	/**
	 * 1단계 - 해리스-라플라스 특징(키포인트) 검출 단계 (교재 p.187 알고리즘 4-3)
	 * 검출된 키포인트는 모두 image_keypoints에 저장된다.
	 */
	SIFT sift;
	std::vector<OctaveSet> octave_sets;
	std::vector<KeyPoint> image_keypoints;

	// TODO #1: sift.cpp의 SIFT::GenerateOctaveSet을 구현하세요.
	OctaveSet octave_set = sift.GenerateOctaveSet(gaussian_image);
	octave_sets.push_back(octave_set);

	std::vector<KeyPoint> octave_keypoint = sift.FindKeyPoints(octave_set);
	// image_keypoints = [image_keypoints] + [octave_keypoint]  (vector concatenation)
	image_keypoints.insert(image_keypoints.end(), octave_keypoint.begin(), octave_keypoint.end());

	for (int i = 0; i < 3; ++i)
	{
		// 마지막에 추가한 OctaveSet을 가져온다.
		OctaveSet octave_set = octave_sets.back();

		cv::Mat centerGaussianMat = octave_set.gaussianMat[3];
		cv::Mat halfSizeCenterGaussianMat;
		cv::resize(centerGaussianMat, halfSizeCenterGaussianMat, cv::Size(centerGaussianMat.cols / 2, centerGaussianMat.rows / 2));
		OctaveSet halfimg_octave_set = sift.GenerateOctaveSet(halfSizeCenterGaussianMat);
		octave_sets.push_back(halfimg_octave_set);

		std::vector<KeyPoint> halfimg_octave_keypoint = sift.FindKeyPoints(halfimg_octave_set);
		// image_keypoints = [image_keypoints] + [halfimg_octave_keypoint]  (vector concatenation)
		image_keypoints.insert(image_keypoints.end(), halfimg_octave_keypoint.begin(), halfimg_octave_keypoint.end());
	}

	// 화면에 키포인트를 그린다.
	sift.DrawKeyPoints(image_keypoints, original_image, keypoint_image);

	/**
	 * 2단계 - SIFT 기술자 추출 단계 (교재 p.258 알고리즘 6-1)
	 * 
	 * 먼저, findDominantOrientation에서는 검출된 키포인트를 각각 접근하여
	 * 지배적인 방향 theta_i를 구하고 dominantOrientation property에 저장한다.
	 * 
	 * 그 다음, makeDescriptor에서 특징 벡터 x_i를 DescriptorVector로 생성해
	 * 그 리스트를 반환한다.
	 */

	// TODO #2: sift.cpp의 SIFT::FindDominantOrientation을 구현하세요.
	sift.FindDominantOrientation(image_keypoints, octave_sets);
	
	// TODO #3: sift.cpp의 SIFT::MakeDescriptor을 구현하세요.
	std::vector<DescriptorVector> descriptor_vectors = sift.MakeDescriptor(image_keypoints, octave_sets);

	// 기술자들 중 첫 8개만 프린트함

	std::cout << "1. 검출된 기술자 개수: " << descriptor_vectors.size() << "개" << std::endl;
	std::cout << "2. 검출된 기술자 마지막 8개 (그보다 적을 경우 일부 출력되지 않음)" << std::endl;

	int print_start_idx = MAX(0, descriptor_vectors.size() - 9);
	int print_end_idx = MAX(0, descriptor_vectors.size() - 1);
	for (int i = print_start_idx; i <= print_end_idx; ++i)
	{
		for (int desc_idx = 0; desc_idx < 8; ++desc_idx)
			// std::cout << descriptor_vectors[i].descriptor[desc_idx] << "\t";
			printf("%0.5f\t", descriptor_vectors[i].descriptor[desc_idx]);
		printf("\n"); // std::cout << std::endl;
	}

	cv::imshow("Original Image", original_image);
	cv::imshow("Keypoint Image", keypoint_image);
	cv::waitKey();

	// 이미지를 파일로 저장한다.
	cv::imwrite("cliff_keypoint.bmp", keypoint_image);
	std::cout << "파일 저장 완료!" << std::endl;
}