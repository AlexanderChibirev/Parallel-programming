#pragma once
#include "IMatrix.h"

class CMatrixSuccessively : IMatrix
{

public:
	CMatrixSuccessively(std::vector<std::vector<float>> matrix);
	std::vector<std::vector<float>> GetInverseMatrix() override;
private:
	CMatrixSuccessively CoFactor();
	float GetDeterminant();
	float SearchDetThenMatrixSizeEqualThree(std::vector<std::vector<float>> &pd);
	float SearchDetThenMatrixSizeEqualFour();
	float SearchDetThenMatrixSizeEqualFive();
private:
	int m_matrixSize;
	std::vector<std::vector<float>> m_baseMatrix;
};

