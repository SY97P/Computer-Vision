/*
 * ����4: Rotation using Homogeneous Matrix
 */

#include "geometry.hpp"
#include "opencv2\opencv.hpp"

void GeometryTransformator::ForwardTransformation(cv::InputArray& srcMat, cv::OutputArray& dstMat)
{
	if (!hasMatrixSet)
		throw "���� ����� ���ǵ��� �ʾҽ��ϴ�. ���� ����� ���� �������ּ���.";

	cv::Mat input_image, output_image;
	
	// ���ڷκ��� Mat �̹��� �ε�
	input_image = srcMat.getMat();
	if (dstMat.empty())
		dstMat.create(input_image.size(), input_image.type());
	output_image = dstMat.getMat();

	/**
	 * input_image�� �̹����� Forward Tranformation�ϴ� �ڵ带 �����ϼ���.
	 * input_image�� H ������ �̿��Ͽ� Transformation�ϰ�, ����� output_image�� �����ϼ���.
	 */
	// ** ���ݺ��� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
	for (int y = 0; y < input_image.rows; y++)
	{
		for (int x = 0; x < input_image.cols; x++)
		{
			// ��ȯ�� ���� ��ġ�� ���Ѵ�. 
			// �Ǽ��� �ݿø��ؼ� ������ �����. 
			int j = (int) (H[0][0] * y + H[1][0] * x);
			int i = (int) (H[0][1] * y + H[1][1] * x);
			
			// ���� ������ ��� ���� �����Ѵ�. 
			// ���� ������ ����� ���� ���� sourc���� desti�� ��ȯ�Ѵ�.
			if ((j >= 0) && (j <= output_image.rows - 1) && (i >= 0) && (i <= output_image.cols - 1))
			{
				output_image.at<uchar>(j, i) = input_image.at<uchar>(y, x);
			}

		}
	}
	// ** ������� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
}

void GeometryTransformator::BackwardTransformation(cv::InputArray& srcMat, cv::OutputArray& dstMat)
{
	if (!hasMatrixSet)
		throw "���� ����� ���ǵ��� �ʾҽ��ϴ�. ���� ����� ���� �������ּ���.";

	cv::Mat input_image, output_image;

	// ���ڷκ��� Mat �̹��� �ε�
	input_image = srcMat.getMat();
	if (dstMat.empty())
		dstMat.create(input_image.size(), input_image.type());
	output_image = dstMat.getMat();

	double H_inverse[3][3] = {};
	InverseHmatrix(H_inverse);

	/**
	 * input_image�� �̹����� Backward Tranformation�ϴ� �ڵ带 �����ϼ���.
	 * input_image�� H_inverse ������ �̿��Ͽ�
	 * GeometryTransformator::ForwardTransformation�� �����ϰ� �����ϸ� �˴ϴ�.
	 */
	// ** ���ݺ��� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **

	/*
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			printf("%f ", H_inverse[i][j]);
		printf("\n");
	}
	*/
	for (int y = 0; y < input_image.rows; y++)
	{
		for (int x = 0; x < input_image.cols; x++)
		{
			// ���⵵ �Ǽ��� ����� ���������� ��ȯ�Ͽ� ����
			int j = (int)(H_inverse[0][0] * y + H_inverse[1][0] * x);
			int i = (int)(H_inverse[0][1] * y + H_inverse[1][1] * x);

			// ���� ������ �Ѿ�� �ʴ� �ȼ��� �ݿ�
			if ((j >= 0) && (j <= output_image.rows - 1) && (i >= 0) && (i <= output_image.cols - 1))
				output_image.at<uchar>(y, x) = input_image.at<uchar>(j, i);
		}
	}
	// ** ������� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
}

void GeometryTransformator::SetMoveMatrix(double y, double x)
{
	ClearMatrix();
	
	H[0][0] = 1;
	H[1][1] = 1;
	H[2][0] = y;
	H[2][1] = x;
	H[2][2] = 1;

	hasMatrixSet = true;
}

void GeometryTransformator::SetRotateMatrix(double degree)
{
	ClearMatrix();

	/**
	 * 3*3 ��� ���� H�� �̵� ���� ����� �ۼ��ϼ���.
	 * ������ ���ڸ��� ���� �Ҵ��ϸ� �˴ϴ�.
	 * cos() �Լ��� sin() �Լ��� ���ڴ� degree�� �ƴ� radian�� �Է¹޴� ���� �����ϰ�
	 * PI ������ Ȱ���Ͽ� ��ȯ�մϴ�.
	 */
	// ** ���ݺ��� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **
	H[0][0] = cos(degree * (PI / 180));
	H[0][1] = -sin(degree * (PI / 180));
	H[0][2] = 0;
	H[1][0] = sin(degree * (PI / 180));
	H[1][1] = cos(degree * (PI / 180));
	H[1][2] = 0;
	H[2][0] = 0;
	H[2][1] = 0;
	H[2][2] = 1;
	
	/*
	rotation Matrix�� ���� ���� �� �������
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%f	", H[i][j]);
		}
		printf("\n");
	}
	*/
	
	// ** ������� �ڵ带 �ۼ��ϼ���. �� ���� ����ø� �� �˴ϴ� **

	hasMatrixSet = true;
}

void GeometryTransformator::InverseHmatrix(double result[3][3])
{
	double determinant = 0;

	for (int i = 0; i < 3; i++)
		determinant += (H[0][i] * (H[1][(i + 1) % 3] * H[2][(i + 2) % 3] - H[1][(i + 2) % 3] * H[2][(i + 1) % 3]));

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result[i][j] = (
				(H[(j + 1) % 3][(i + 1) % 3] * H[(j + 2) % 3][(i + 2) % 3]) -
				(H[(j + 1) % 3][(i + 2) % 3] * H[(j + 2) % 3][(i + 1) % 3])
				) / determinant;
		}
	}
}

void GeometryTransformator::ClearMatrix()
{
	memset(H, 0, sizeof(H));
	hasMatrixSet = false;
}
