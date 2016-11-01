#pragma once
#include "IMatrix.h"

class CMatrixInverseSuccessively : IMatrix
{

public:
	CMatrixInverseSuccessively(std::vector<std::vector<float>> matrix);
	std::vector<std::vector<float>> GetInverseMatrix() override;
private:
	CMatrixInverseSuccessively CoFactor();
	float GetDeterminant();
	float SearchDetThenMatrixSizeEqualThree(std::vector<std::vector<float>> &pd);
	float SearchDetThenMatrixSizeEqualFour();
	float SearchDetThenMatrixSizeEqualFive();
private:
	int m_matrixSize;
	std::vector<std::vector<float>> m_baseMatrix;
};

