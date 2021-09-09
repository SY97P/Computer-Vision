/*
 * 과제11: Panorama
 */

#include <chrono>
#include <cstdlib>
#include <ctime>

#include "opencv2/opencv.hpp"
#include "panorama/pransac.hpp"
#include "panorama/moravec.hpp"
#include "panorama/hog.hpp"
#include "panorama/distance_metric.hpp"

using namespace panorama;

// 두 포인트를 거리순으로 오름차순 정렬해주는 std::sort의 헬퍼 함수이다.
bool sort_by_distance_ascending(PointSet& a, PointSet& b)
{
	return b.distance > a.distance;
}

// 좌표 사이의 거리를 느슨하게 계산한다.
// 두 좌표가 y, x 좌표 모두 loosenPx보다 가깝게 붙어있다면
// 해당 좌표는 동일한 좌표로 판단하고 true를 반환한다.
bool compare_ptr_loosen(const cv::Point& a, const cv::Point& b, const double loosenPx = 2)
{
	if (abs(a.x - b.x) < loosenPx && abs(a.y - b.y) < loosenPx)
		return true;
	return false;
}

int main()
{
	// 랜덤한 컬러를 위한 초기화 코드
	std::srand(static_cast<unsigned int>(std::time(0)));

	cv::Mat first_image_color, second_image_color, first_image, second_image;
	cv::Mat first_image_gaussian, second_image_gaussian;
	cv::Mat attached_image;

	first_image_color = cv::imread("landscape-left.bmp", cv::IMREAD_COLOR);
	second_image_color = cv::imread("landscape-right.bmp", cv::IMREAD_COLOR);

	cv::cvtColor(first_image_color, first_image, cv::COLOR_BGR2GRAY);
	cv::cvtColor(second_image_color, second_image, cv::COLOR_BGR2GRAY);

	assert(first_image.rows > 200 && first_image.cols > 200);
	assert(second_image.rows > 200 && second_image.cols > 200);

	/**
	 * 1단계 - 모라벡 알고리즘을 이용한 Keypoint 검출
	 * CreateConfidence()와 FindKeyPoint()를 구현하세요.
	 */
	Moravec moravec_first(first_image);
	moravec_first.CreateConfidence();
	moravec_first.FindKeyPoint();

	Moravec moravec_second(second_image);
	moravec_second.CreateConfidence();
	moravec_second.FindKeyPoint();

	/**
	 * 2단계 - 그레이디언트의 히스토그램을 통한 특징 추출
	 */
	HOG hog;
	std::vector<HOGPair> first_image_hog = hog.GetHOG(first_image, moravec_first);
	std::vector<HOGPair> second_image_hog = hog.GetHOG(second_image, moravec_second);
	
	/**
	 * 3단계 - 추출한 특징간 거리 비교 및 두 이미지간 특징 Pair 검출
	 * ExtractMinDistance 내 CalculateDistance 함수를 구현하세요.
	 */
	DistanceCalculator euclidean_distance;
	euclidean_distance.ExtractMinDistance(first_image_hog, second_image_hog);
	// euclidean_distance.DisplayPairLine(first_image, second_image);
	std::vector<PointSet> distance_pair = euclidean_distance.GetDistancePair();

	/**
	 * 4단계 - 변환 행렬 구하기 단계
	 * 
	 * 구한 distance_pair를 이용하여 변환 행렬을 구한다.
	 * 교재 p.325의 알고리즘 7-9를 이용하여 기하 변환 행렬 T를 추정한다.
	 * 
	 * 기하변환 T는 이미지#1을 이미지#2에 해당하는 점으로 변환하는 행렬이다.
	 * 이 특성을 이용하여, 후방 기하 변환을 이용하여 새로운 이미지를 만들어 내도록 한다.
	 * 
	 * GetBestRotationMat 함수 내 GetHomogeneousMatrix 함수를 구현하세요.
	 */
	PRANSAC ransac(distance_pair);
	ransac.GetBestRotationMat();
	ransac.AttachImage(first_image_color, second_image_color, attached_image);

	// 화면에 결과를 표시한다.
	cv::imshow("Attached Image", attached_image);
	cv::waitKey();

	// 이미지를 파일로 저장한다.
	cv::imwrite("attached-landscape.bmp", attached_image);
	std::cout << "파일 저장 완료!" << std::endl;
}